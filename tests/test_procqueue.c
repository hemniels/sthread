#include "../datastruct/dll.h"
#include "../datastruct/procqueue.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main() {
    printf("Running tests for Process Queue...\n");

    DoublyLinkedList* queue = create_doubly_linked_list();
    assert(is_empty(queue) == 1);

    Process p1 = {1, "Process 1"};
    Process p2 = {2, "Process 2"};
    Process p3 = {3, "Process 3"};

    enqueue_process(queue, p1);
    enqueue_process(queue, p2);
    enqueue_process(queue, p3);
    assert(is_empty(queue) == 0);

    print_process_queue(queue);

    Process p = dequeue_process(queue);
    assert(p.pid == 1);
    assert(strcmp(p.name, "Process 1") == 0);

    print_process_queue(queue);

    p = peek_process(queue);
    assert(p.pid == 2);
    assert(strcmp(p.name, "Process 2") == 0);

    destroy_doubly_linked_list(queue);

    printf("All tests passed!\n");
    return 0;
}
