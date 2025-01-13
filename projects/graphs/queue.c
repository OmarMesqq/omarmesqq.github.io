#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

Queue* create_queue() {
    Queue* q = (Queue*) malloc(sizeof(Queue));
    if (!q) {
        printf("Memory allocation failed during queue creation.\n");
        return NULL;
    }

    q->front = NULL;
    q->back = NULL;
    
    return q;
}

void enqueue(Queue* q, Neighbour* nb) {
    if (!q || !nb) return;
    QueueNode* qn = (QueueNode*) malloc(sizeof(QueueNode));
    if (!qn) {
        printf("Memory allocation failed during queue node creation in enqueue.\n");
        return;
    }
    qn->nb = nb;
    qn->next = NULL;

    if (!q->front || !q->back) {
        q->front = qn;
        q->back = qn;
        return;
    }

    q->back->next = qn;
    q->back = qn;
}

QueueNode* dequeue(Queue* q) {
    if (!q) return NULL;

    QueueNode* first = q->front;
    
    if (first) {
        q->front = q->front->next;
    } else {
        q->front = NULL;
    }
    
    return first;
}

int is_queue_empty(Queue* q) {
    if (!q) {
        printf("NULL queue can't be evaluated.\n");
        return -1;
    };
    
    if (q->front) {
        return 0;
    }
    return 1;
}



void free_queue(Queue* q) {
    if (!q) return;

    while (!is_queue_empty(q)) {
        QueueNode* current = dequeue(q);
        free(current);
    }
    free(q);
}
