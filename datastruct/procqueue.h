#ifndef PROCQUEUE_H
#define PROCQUEUE_H

#include "../datastruct/dll.h"

// Definition der Prozessstruktur
typedef struct {
    int pid;  // Prozess ID
    char name[256];  // Prozessname
} Process;

// Funktionsprototypen für die Prozess-Warteschlange
void enqueue_process(DoublyLinkedList* queue, Process process);
Process dequeue_process(DoublyLinkedList* queue);
Process peek_process(DoublyLinkedList* queue);
int is_empty(DoublyLinkedList* queue);
void print_process_queue(DoublyLinkedList* queue);

#endif // PROCQUEUE_H
