#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "../datastruct/sthread.h"

typedef struct {
    sthread_t *run_queue;
    sthread_t *blocked_queue;
    sthread_t *zombie_queue;
} Scheduler;

#define BLOCKED 1
#define RUNNING 0

/* Initialize the scheduler */
void scheduler_init(Scheduler *scheduler);

/* Schedule the next process */
void scheduler_run(Scheduler *scheduler);

/* Block the current process */
void scheduler_block(Scheduler *scheduler);

/* Unblock a process */
void scheduler_unblock(Scheduler *scheduler, int pid);

/* Add a process to the run queue */
void scheduler_add(Scheduler *scheduler, sthread_t *process);

/* Remove a process from the run queue */
sthread_t *scheduler_remove(Scheduler *scheduler);

/* Function to determine which file descriptor is ready */
int whichisready(int fd1, int fd2);

#endif
