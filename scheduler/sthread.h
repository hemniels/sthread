#ifndef STHREAD_H
#define STHREAD_H

#include <ucontext.h>
#include <stddef.h>
#include <unistd.h>  // Include for ssize_t

#define STACK_SIZE 8192
#define BLOCKED 1
#define RUNNING 0

typedef void (*sthr_proc_func)(void);

typedef struct sthread_t {
    int pid;
    int ppid;
    ucontext_t context;
    int status;
    int exited;
    void *stack;
    struct sthread_t *next;
} sthread_t;

void sthr_init(sthr_proc_func f);
int sthr_spawn(sthr_proc_func f);
void sthr_yield();
void sthr_exit(int status);
int sthr_waitpid(int cpid, int *status);
int sthr_getpid();
int sthr_getppid();
ssize_t sthr_read(int fd, void *buf, size_t count);

#endif
