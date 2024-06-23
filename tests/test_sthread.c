#include "../datastruct/sthread.h"
#include <stdio.h>
#include <stdlib.h>

void child_process() {
    printf("Child process is running!\n");
    sthr_yield();
    printf("Child process resumed after yield!\n");
    sthr_exit(EXIT_SUCCESS);
}

int main() {
    // Initialize the sthread library with the child_process function
    sthr_init(child_process);

    // Spawn another child process
    int child_pid = sthr_spawn(child_process);
    if (child_pid != -1) {
        printf("Spawned child process with PID: %d\n", child_pid);
    } else {
        fprintf(stderr, "Failed to spawn child process\n");
        return EXIT_FAILURE;
    }

    // Yield the CPU to the child processes
    sthr_yield();
    printf("Main process resumed after yielding to child processes\n");

    // Wait for the child process to finish
    int status;
    if (sthr_waitpid(child_pid, &status) == 0) {
        printf("Child process with PID %d exited with status %d\n", child_pid, status);
    } else {
        fprintf(stderr, "Failed to wait for child process\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
