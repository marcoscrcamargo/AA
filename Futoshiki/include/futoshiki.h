#ifndef FUTOSHIKI_H
#define FUTOSHIKI_H

#include "priority_queue.h"

#define UP 0 // (-1, 0)
#define DOWN 1 // (+1, 0)
#define LEFT 2 // (0, -1)
#define RIGHT 3 // (0, +1)

typedef struct Board Board;
typedef struct Cell Cell;

struct Cell{
	int value, n, x, y;
	int *possibility;
	bool *greater;
	bool *lesser;
};

struct Board{
	PQNode ***ref;
	Cell ***cell;
	int d;
};

Board *read_board();

void generate_possibilities(Board *);

void print_board(const Board *);

void free_board(Board *);

#endif