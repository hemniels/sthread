#include "sthread.h"
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>

static sthread_t *run_queue = NULL;
static sthread_t *blocked_queue = NULL;
static sthread_t *zombie_queue = NULL;
static sthread_t *current_thread = NULL;
static int next_pid = 1;

void scheduler() {
    while (run_queue != NULL || blocked_queue != NULL) {
        if (run_queue == NULL) {
            int fd = -1; // Placeholder for actual file descriptor
            whichisready(fd, fd);
        } else {
            sthread_t *prev_thread = current_thread;
            current_thread = run_queue;
            run_queue = run_queue->next;
            current_thread->next = NULL;

            if (prev_thread) {
                swapcontext(&prev_thread->context, &current_thread->context);
            } else {
                setcontext(&current_thread->context);
            }

            if (current_thread->exited) {
                current_thread->next = zombie_queue;
                zombie_queue = current_thread;
            } else if (current_thread->status == BLOCKED) {
                current_thread->next = blocked_queue;
                blocked_queue = current_thread;
            } else {
                sthread_t *temp = run_queue;
                while (temp && temp->next) {
                    temp = temp->next;
                }
                if (temp) {
                    temp->next = current_thread;
                } else {
                    run_queue = current_thread;
                }
            }
        }
    }
}

int whichisready(int fd1, int fd2) {
    int maxfd;
    int nfds;
    fd_set readset;

    if ((fd1 < 0) || (fd1 >= FD_SETSIZE) || (fd2 < 0) || (fd2 >= FD_SETSIZE)) {
        errno = EINVAL;
        return -1;
    }
    maxfd = (fd1 > fd2) ? fd1 : fd2;
    FD_ZERO(&readset);
    FD_SET(fd1, &readset);
    FD_SET(fd2, &readset);
    nfds = select(maxfd + 1, &readset, NULL, NULL, NULL);
    if (nfds == -1) {
        return -1;
    }
    if (FD_ISSET(fd1, &readset)) {
        return fd1;
    }
    if (FD_ISSET(fd2, &readset)) {
        return fd2;
    }
    errno = EINVAL;
    return -1;
}

ssize_t sthr_read(int fd, void *buf, size_t count) {
    ssize_t result;
    while ((result = read(fd, buf, count)) == -1 && errno == EAGAIN) {
        sthr_yield();
    }
    return result;
}

int sthr_spawn(sthr_proc_func f) {
    sthread_t *new_thread = malloc(sizeof(sthread_t));
    if (new_thread == NULL) {
        return -1;
    }
    new_thread->stack = malloc(STACK_SIZE);
    if (new_thread->stack == NULL) {
        free(new_thread);
        return -1;
    }
    getcontext(&new_thread->context);
    new_thread->context.uc_link = NULL;
    new_thread->context.uc_stack.ss_sp = new_thread->stack;
    new_thread->context.uc_stack.ss_size = STACK_SIZE;
    new_thread->context.uc_stack.ss_flags = 0;
    makecontext(&new_thread->context, f, 0);
    new_thread->pid = next_pid++;
    new_thread->ppid = current_thread ? current_thread->pid : 0;
    new_thread->status = RUNNING;
    new_thread->exited = 0;
    new_thread->next = NULL;

    if (run_queue == NULL) {
        run_queue = new_thread;
    } else {
        sthread_t *temp = run_queue;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_thread;
    }

    return new_thread->pid;
}

void sthr_yield() {
    if (run_queue == NULL) {
        return;
    }
    sthread_t *prev_thread = current_thread;
    if (prev_thread) {
        sthread_t *temp = run_queue;
        while (temp && temp->next) {
            temp = temp->next;
        }
        if (temp) {
            temp->next = prev_thread;
        } else {
            run_queue = prev_thread;
        }
        prev_thread->next = NULL;
    }
    scheduler();
}

void sthr_exit(int status) {
    current_thread->status = status;
    current_thread->exited = 1;
    scheduler();
}

int sthr_waitpid(int cpid, int *status) {
    sthread_t *temp;
    while (1) {
        temp = run_queue;
        while (temp != NULL) {
            if (temp->pid == cpid && temp->exited) {
                *status = temp->status;
                return 0;
            }
            temp = temp->next;
        }
        sthr_yield();
    }
    return -1;
}

void sthr_init(sthr_proc_func f) {
    sthread_t main_thread;
    main_thread.pid = next_pid++;
    main_thread.ppid = 0;
    main_thread.context.uc_link = NULL;
    main_thread.context.uc_stack.ss_sp = malloc(STACK_SIZE);
    main_thread.context.uc_stack.ss_size = STACK_SIZE;
    main_thread.context.uc_stack.ss_flags = 0;
    main_thread.status = RUNNING;
    main_thread.exited = 0;
    main_thread.next = NULL;

    current_thread = &main_thread;
    sthr_spawn(f);
    while (run_queue != NULL) {
        sthr_yield();
    }
    _exit(0);
}

int sthr_getpid() {
    return current_thread ? current_thread->pid : -1;
}

int sthr_getppid() {
    return current_thread ? current_thread->ppid : -1;
}
