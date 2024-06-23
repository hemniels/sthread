#include "../datastruct/sthread.h"
#include <assert.h>
#include <stdio.h>
#include <unistd.h>

void test_process() {
    for (int i = 0; i < 5; i++) {
        printf("Process running: %d\n", sthr_getpid());
        sthr_yield();
    }
    sthr_exit(0);
}

void test_scheduler() {
    sthr_init(test_process);
    printf("Scheduler test passed.\n");
}

int main() {
    test_scheduler();
    return 0;
}
