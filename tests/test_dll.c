#include "../datastruct/dll.h"
#include <assert.h>
#include <stdio.h>

int main() {
    printf("Running tests for DLL...\n");

    DoublyLinkedList* list = create_doubly_linked_list();
    assert(list->size == 0);

    insert_at_head(list, 10);
    assert(list->head->data == 10);
    assert(list->tail->data == 10);
    assert(list->size == 1);

    insert_at_tail(list, 20);
    assert(list->head->data == 10);
    assert(list->tail->data == 20);
    assert(list->size == 2);

    insert_at_head(list, 5);
    assert(list->head->data == 5);
    assert(list->tail->data == 20);
    assert(list->size == 3);

    print_list(list);

    delete_node(list, list->head->next);
    assert(list->head->data == 5);
    assert(list->tail->data == 20);
    assert(list->size == 2);

    print_list(list);

    destroy_doubly_linked_list(list);

    printf("All tests passed!\n");
    return 0;
}
