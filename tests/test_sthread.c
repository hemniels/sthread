#include "../datastruct/sthread.h"
#include <stdio.h>

void child1() {
    printf("Child 1: Start\n");
    sthr_yield();
    printf("Child 1: End\n");
    sthr_exit(0);
}

void child2() {
    printf("Child 2: Start\n");
    sthr_yield();
    printf("Child 2: End\n");
    sthr_exit(0);
}

void parent() {
    int status;
    printf("Parent: Creating Child 1\n");
    int pid1 = sthr_spawn(child1);
    printf("Parent: Creating Child 2\n");
    int pid2 = sthr_spawn(child2);
    printf("Parent: Waiting for Child 1\n");
    sthr_waitpid(pid1, &status);
    printf("Parent: Child 1 exited with status %d\n", status);
    printf("Parent: Waiting for Child 2\n");
    sthr_waitpid(pid2, &status);
    printf("Parent: Child 2 exited with status %d\n", status);
    sthr_exit(0);
}

int main() {
    sthr_init(parent);
    return 0;
}
