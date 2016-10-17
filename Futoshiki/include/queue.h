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