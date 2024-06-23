#include "sthread.h"

#define STACK_SIZE 8192

typedef struct sthread_t {
    int pid;
    int ppid;
    ucontext_t context;
    int status;
    int exited;
    void *stack;
    struct sthread_t *next;
} sthread_t;

static sthread_t *run_queue = NULL;
static sthread_t *current_thread = NULL;
static int next_pid = 1;

void sthr_schedule() {
    if (run_queue == NULL) {
        return;
    }
    sthread_t *prev_thread = current_thread;
    current_thread = run_queue;
    run_queue = run_queue->next;
    current_thread->next = NULL;

    if (prev_thread) {
        swapcontext(&prev_thread->context, &current_thread->context);
    } else {
        setcontext(&current_thread->context);
    }
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
    new_thread->status = 0;
    new_thread->exited = 0;
    new_thread->next = NULL;

    if (run_queue == NULL) {
        run_queue = new_thread;
    } else {
        sthread_t *tmp = run_queue;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = new_thread;
    }

    return new_thread->pid;
}

void sthr_yield() {
    if (run_queue == NULL) {
        return;
    }
    sthread_t *prev_thread = current_thread;
    if (prev_thread) {
        if (run_queue == NULL) {
            run_queue = prev_thread;
        } else {
            sthread_t *tmp = run_queue;
            while (tmp->next != NULL) {
                tmp = tmp->next;
            }
            tmp->next = prev_thread;
        }
        prev_thread->next = NULL;
    }
    sthr_schedule();
}

void sthr_exit(int status) {
    current_thread->status = status;
    current_thread->exited = 1;
    sthr_schedule();
}

int sthr_waitpid(int cpid, int *status) {
    sthread_t *tmp = run_queue;
    sthread_t *parent = current_thread;

    while (tmp != NULL) {
        if (tmp->pid == cpid && tmp->exited) {
            *status = tmp->status;
            return 0;
        }
        tmp = tmp->next;
    }

    while (1) {
        tmp = run_queue;
        while (tmp != NULL) {
            if (tmp->pid == cpid && tmp->exited) {
                *status = tmp->status;
                return 0;
            }
            tmp = tmp->next;
        }
        sthr_yield();
    }
    return -1;
}

ssize_t sthr_read(int fd, void *buf, size_t count) {
    ssize_t result;
    while ((result = read(fd, buf, count)) == -1 && errno == EAGAIN) {
        sthr_yield();
    }
    return result;
}

void sthr_init(sthr_proc_func f) {
    sthread_t main_thread;
    main_thread.pid = next_pid++;
    main_thread.ppid = 0;
    main_thread.context.uc_link = NULL;
    main_thread.context.uc_stack.ss_sp = malloc(STACK_SIZE);
    main_thread.context.uc_stack.ss_size = STACK_SIZE;
    main_thread.context.uc_stack.ss_flags = 0;
    main_thread.status = 0;
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
