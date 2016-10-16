/* Victor Forbes - 9293394 */

#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "queue.h"

struct QNode{
	void *element;
	QNode *next;
	QNode *previous;
};

struct Queue{
	QNode *front;
	QNode *back;
	int size;
	int element_size;
};

/* Creates a node. */
QNode *queue_node_new(const void *, int);

/* Erases a node from the Queue. */
void queue_node_delete(QNode *);

Queue *queue_new(int element_size){
	Queue *q = (Queue *)malloc(sizeof(Queue));

	q->element_size = element_size;
	q->front = NULL;
	q->back = NULL;
	q->size = 0;

	return q;
}

void queue_clear(Queue *q){
	while (q->size){
		queue_pop(q);
	}
}

void queue_delete(Queue *q){
	queue_clear(q);
	free(q);
}

QNode *queue_push(Queue *q, const void *element){
	if (q->size){
		q->back->next = queue_node_new(element, q->element_size);
		q->back->next->previous = q->back;
		q->back = q->back->next;
	}
	else{
		q->back = q->front = queue_node_new(element, q->element_size);
	}

	q->size++;

	return q->back;
}

void queue_replace(Queue *q, QNode *ref, const void *element){
	memcpy(ref->element, element, q->element_size);
}

void *queue_front(const Queue *q){
	return q->size ? q->front->element : NULL;
}

void queue_pop(Queue *q){
	if (q->size){
		if (q->front->next){
			q->front = q->front->next;
			queue_node_delete(q->front->previous);
		}
		else{
			queue_node_delete(q->front);
			q->front = NULL;
			q->back = NULL;
		}

		q->size--;
	}
}

bool queue_empty(const Queue *q){
	return !q->size;
}

int queue_size(const Queue *q){
	return q->size;
}

/* --- Funções auxiliares --- */

QNode *queue_node_new(const void *element, int element_size){
	QNode *n = (QNode *)malloc(sizeof(QNode));

	n->element = malloc(element_size);
	memcpy(n->element, element, element_size);

	n->next = NULL;
	n->previous = NULL;

	return n;
}

void queue_node_delete(QNode *n){
	free(n->element);
	free(n);
}