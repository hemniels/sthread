#include "sthread.h"

#define STACK_SIZE 8192

static ucontext_t main_context;
static ucontext_t thread_context;
static sthr_proc_func thread_func = NULL;

static void thread_start() {
    if (thread_func) {
        thread_func();
        sthr_exit(EXIT_SUCCESS);
    }
}

int sthr_spawn(sthr_proc_func f) {
    getcontext(&thread_context);
    thread_func = f;
    thread_context.uc_stack.ss_sp = malloc(STACK_SIZE);
    thread_context.uc_stack.ss_size = STACK_SIZE;
    thread_context.uc_link = &main_context;
    makecontext(&thread_context, thread_start, 0);

    swapcontext(&main_context, &thread_context);

    return 0; // Return child process ID if needed
}

void sthr_yield() {
    swapcontext(&thread_context, &main_context);
}

void sthr_exit(int status) {
    if (thread_context.uc_stack.ss_sp)
        free(thread_context.uc_stack.ss_sp);
    thread_func = NULL;
    setcontext(&main_context);
}

ssize_t sthr_read(int fd, void *buf, size_t count) {
    // Implement read functionality
    return read(fd, buf, count);
}

int sthr_waitpid(int cpid, int *status) {
    // Implement waitpid functionality
    return waitpid(cpid, status, 0);
}

void sthr_init(sthr_proc_func f) {
    thread_func = f;
    getcontext(&main_context);
    sthr_spawn(f);
}

int sthr_getpid() {
    return getpid();
}

int sthr_getppid() {
    return getppid();
}
