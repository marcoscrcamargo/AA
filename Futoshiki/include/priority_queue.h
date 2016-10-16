/*Copyright © 2016 - Marcos Cesar Ribeiro de Camargo (9278045), Victor Luiz Roquete Forbes (9293394)

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
along with Futoshiki.  If not, see <http://www.gnu.org/licenses/>.*/

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