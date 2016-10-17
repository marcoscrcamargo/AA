/* Copyright © 2016 - Marcos Cesar Ribeiro de Camargo (9278045), Victor Luiz Roquete Forbes (9293394)

This file is part of Futoshiki.

Futoshiki is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Futoshiki is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Futoshiki.  If not, see <http://www.gnu.org/licenses/>. */

#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "priority_queue.h"

#define parent(x) ((x - 1) / 2) // Pai da posição x.
#define left_child(x) (2 * x + 1) // Filho da esquerda da posição x.
#define right_child(x) (2 * x + 2) // Filho da direita da posição x.

struct PQNode{
	int pos; // Posição no vetor da Heap.
	void *value; // Valor armazenado.
};

struct PriorityQueue{
	int n; // Número de elementos.
	int size; // Tamanho de cada elemento.
	PQNode **v; // Vetor de nós.
	int (*compare)(const void *, const void *); // Função de comparação.
};

// Aumentando o nó ref de posição na Heap.
void priority_queue_increase(PriorityQueue *, PQNode *);

// Reduzindo o nó ref de posição na Heap.
void priority_queue_decrease(PriorityQueue *, PQNode *);

// Criando um novo nó.
PQNode *node_new(const void *, int, int);

// Deletando um nó.
void node_delete(PQNode *);

PriorityQueue *priority_queue_new(int size, int (*compare)(const void *, const void *)){
	PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));

	// Inicializando.
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

	// Substituindo o elemento.
	memcpy(ref->value, x, pq->size);

	// Subindo ou descendo o novo elemento.
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

/* --- Funções auxiliares --- */

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