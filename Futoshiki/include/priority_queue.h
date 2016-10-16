/* Victor Forbes - 9293394 */

#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "utils.h"

typedef struct PriorityQueue PriorityQueue;
typedef struct PQNode PQNode;

PriorityQueue *priority_queue_new(int, int (*)(const void *, const void *));

PQNode *priority_queue_push(PriorityQueue *, const void *);

void priority_queue_replace(PriorityQueue *, PQNode *, const void *);

void *priority_queue_top(const PriorityQueue *);

void priority_queue_pop(PriorityQueue *);

int priority_queue_size(const PriorityQueue *);

bool priority_queue_empty(const PriorityQueue *);

void priority_queue_delete(PriorityQueue *);

// Aux
void priority_queue_print(PriorityQueue *, void (*)(const void *));

#endif