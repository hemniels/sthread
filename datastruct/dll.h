#ifndef DLL_H
#define DLL_H

#include <stddef.h>

// Definition der Knotenstruktur
typedef struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
} Node;

// Definition der Liste
typedef struct {
    Node* head;
    Node* tail;
    size_t size;
} DoublyLinkedList;

// Funktionsprototypen
DoublyLinkedList* create_doubly_linked_list();
void insert_at_head(DoublyLinkedList* list, int data);
void insert_at_tail(DoublyLinkedList* list, int data);
void delete_node(DoublyLinkedList* list, Node* node);
void destroy_doubly_linked_list(DoublyLinkedList* list);
void print_list(DoublyLinkedList* list);

#endif // DLL_H
