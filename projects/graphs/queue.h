#ifndef QUEUE_H
#define QUEUE_H
#include "graph.h"

typedef struct _queueNode {
    Neighbour* nb;
    QueueNode* next;
} QueueNode;

typedef struct _queue {
    QueueNode* front;
    QueueNode* back;
} Queue;


Queue* create_queue();
void enqueue(Queue* Queue, Neighbour* nb);
QueueNode* dequeue(Queue* Queue);
int is_queue_empty(Queue* Queue);
void free_queue(Queue* Queue);
#endif