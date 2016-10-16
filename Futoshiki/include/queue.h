/* Victor Forbes - 9293394 */

#ifndef QUEUE_H
#define QUEUE_H

#include "utils.h"

typedef struct Queue Queue;
typedef struct QNode QNode;

/* Creates a Queue for elements of size "element_size". */
Queue *queue_new(int);

/* Erases every element in the Queue. */
void queue_clear(Queue *);

/* Destroys Queue. */
void queue_delete(Queue *);

/* Inserts an element at the back of the Queue. */
QNode *queue_push(Queue *, const void *);

/* Replaces an element. */
void queue_replace(Queue *, QNode *, const void *);

/* Returns a reference to the element that is on the front of the Queue. */
void *queue_front(const Queue *);

/* Erases the back element from the Queue. */
void queue_pop(Queue *);

/* Returns true if the Queue is empty and false otherwise. */
bool queue_empty(const Queue *);

/* Returns the amount of elements currently in the Queue. */
int queue_size(const Queue *);

#endif