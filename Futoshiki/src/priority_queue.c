/* Victor Forbes - 9293394 */

#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "priority_queue.h"

#define parent(x) ((x - 1) / 2)
#define left_child(x) (2 * x + 1)
#define right_child(x) (2 * x + 2)

struct PQNode{
	int pos;
	void *value;
};

struct PriorityQueue{
	int n;
	int size;
	PQNode **v;
	int (*compare)(const void *, const void *);
};

void priority_queue_increase(PriorityQueue *, PQNode *);

void priority_queue_decrease(PriorityQueue *, PQNode *);

PQNode *node_new(const void *, int, int);

void node_delete(PQNode *);

PriorityQueue *priority_queue_new(int size, int (*compare)(const void *, const void *)){
	PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));

	pq->n = 0;
	pq->v = NULL;
	pq->size = size;
	pq->compare = compare;

	return pq;
}

PQNode *priority_queue_push(PriorityQueue *pq, const void *x){
    int pos = pq->n;
    PQNode *ref;

    // Colocando o novo elemento no final.
    pq->n++;
    pq->v = (PQNode **)realloc(pq->v, pq->n * sizeof(PQNode *));
    pq->v[pos] = node_new(x, pq->size, pos);

    ref = pq->v[pos];

    // Subindo o nó criado.
    priority_queue_increase(pq, ref);

    return ref;
}

void priority_queue_replace(PriorityQueue *pq, PQNode *ref, const void *x){
	int pos = ref->pos;

	memcpy(ref->value, x, pq->size);

	if (pos > 0 and pq->compare(pq->v[parent(pos)]->value, x) < 0){
		priority_queue_increase(pq, ref);
	}
	else{
		priority_queue_decrease(pq, ref);
	}
}

void *priority_queue_top(const PriorityQueue *pq){
	return pq->n ? pq->v[0]->value : NULL;
}

void priority_queue_pop(PriorityQueue *pq){
	if (pq->n){
		pq->n--;

		// Trocando o último pelo primeiro.
		swap(pq->v, pq->v + pq->n, sizeof(PQNode *));

		// Apagando o último nó.
		node_delete(pq->v[pq->n]);
		pq->v = (PQNode **)realloc(pq->v, pq->n * sizeof(PQNode *));
		
		if (pq->n){
			// Trocando o topo pelo último elemento.
			pq->v[0]->pos = 0;

			priority_queue_decrease(pq, pq->v[0]);
		}
	}
}

int priority_queue_size(const PriorityQueue *pq){
	return pq->n;
}

bool priority_queue_empty(const PriorityQueue *pq){
	return !pq->n;
}

void priority_queue_delete(PriorityQueue *pq){
	int i;

	for (i = 0; i < pq->n; i++){
		node_delete(pq->v[i]);
	}

	free(pq->v);
	free(pq);
}

PQNode *node_new(const void *value, int size, int pos){
	PQNode *n = (PQNode *)malloc(sizeof(PQNode));

	n->value = malloc(size);
	memcpy(n->value, value, size);

	n->pos = pos;

	return n;
}

void node_delete(PQNode *n){
	free(n->value);
	free(n);
}

/* --- Funções auxiliares --- */

void priority_queue_increase(PriorityQueue *pq, PQNode *ref){
	int pos = ref->pos;

	// Subindo o novo elemento até sua posição correta.
    while (pos > 0 and pq->compare(pq->v[parent(pos)]->value, pq->v[pos]->value) < 0){ 
		// Atualizando as posições dos nós.
		pq->v[pos]->pos = parent(pos);
		pq->v[parent(pos)]->pos = pos;

		swap(pq->v + parent(pos), pq->v + pos, sizeof(PQNode *));

        pos = parent(pos);
    }
}

void priority_queue_decrease(PriorityQueue *pq, PQNode *ref){
	int pos, newpos;

	newpos = ref->pos;
	pos = -1;

	// Descendo o elemento até sua posição correta.
	while (newpos != pos){
		pos = newpos;

		if (left_child(pos) < pq->n and pq->compare(pq->v[newpos]->value, pq->v[left_child(pos)]->value) < 0){
			newpos = left_child(pos);
		}

		if (right_child(pos) < pq->n and pq->compare(pq->v[newpos]->value, pq->v[right_child(pos)]->value) < 0){
			newpos = right_child(pos);
		}

		// Atualizando as posições dos nós.
		pq->v[pos]->pos = newpos;
		pq->v[newpos]->pos = pos;

		swap(pq->v + pos, pq->v + newpos, sizeof(PQNode *));
	}
}