/* Victor Forbes - 9293394 */

#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "utils.h"

typedef struct PriorityQueue PriorityQueue;
typedef struct PQNode PQNode;

/* Creates a new Priority Queue. */
PriorityQueue *priority_queue_new(int, int (*)(const void *, const void *));

/* Destroys a Priority Queue. */
void priority_queue_delete(PriorityQueue *);

/* Inserts an element into the Priority Queue and returns a reference to its container. */
PQNode *priority_queue_push(PriorityQueue *, const void *);

/* Replaces an element with another one. */
void priority_queue_replace(PriorityQueue *, PQNode *, const void *);

/* Returns a pointer to the object on top of the Heap. */
void *priority_queue_top(const PriorityQueue *);

/* Removes the top element. */
void priority_queue_pop(PriorityQueue *);

/* Returns the size of the Priority Queue. */
int priority_queue_size(const PriorityQueue *);

/* Returns true if the Priority Queue is empty. */
bool priority_queue_empty(const PriorityQueue *);

#endif