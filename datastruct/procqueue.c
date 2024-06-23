#include "procqueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void enqueue_process(DoublyLinkedList* queue, Process process) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    void* data = malloc(sizeof(Process));
    if (data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(data, &process, sizeof(Process));
    new_node->data = data;
    new_node->prev = queue->tail;
    new_node->next = NULL;
    if (queue->tail != NULL) {
        queue->tail->next = new_node;
    } else {
        queue->head = new_node;
    }
    queue->tail = new_node;
    queue->size++;
}

Process dequeue_process(DoublyLinkedList* queue) {
    if (queue->head == NULL) {
        fprintf(stderr, "Queue is empty\n");
        exit(EXIT_FAILURE);
    }
    Node* node = queue->head;
    Process process = *(Process*)node->data;
    queue->head = node->next;
    if (queue->head != NULL) {
        queue->head->prev = NULL;
    } else {
        queue->tail = NULL;
    }
    free(node->data);
    free(node);
    queue->size--;
    return process;
}

Process peek_process(DoublyLinkedList* queue) {
    if (queue->head == NULL) {
        fprintf(stderr, "Queue is empty\n");
        exit(EXIT_FAILURE);
    }
    return *(Process*)queue->head->data;
}

void print_process_queue(DoublyLinkedList* queue) {
    Node* current = queue->head;
    while (current != NULL) {
        Process* process = (Process*)current->data;
        printf("Process ID: %d, Name: %s\n", process->pid, process->name);
        current = current->next;
    }
    printf("\n");
}
