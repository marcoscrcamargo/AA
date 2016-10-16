#include <stdlib.h>
#include <stdio.h>

#include "utils.h"
#include "futoshiki.h"
#include "priority_queue.h"

Board *create_board(int);

Cell *create_cell(int, int, int);

void free_cell(Cell *);

int relative_position(int, int, int, int);

Board *read_board(){
	int d, r, i, j, x1, y1, x2, y2;
	Board *b;

	// Lendo a dimensão da matriz e o número de desigualdades.
	scanf("%d%d", &d, &r);

	// Alocando um tabuleiro de dimensão d vazio.
	b = create_board(d);

	// Lendo os valores do tabuleiro.
	for (i = 0; i < d; i++){
		for (j = 0; j < d; j++){
			scanf("%d", &b->cell[i][j]->value);
		}
	}

	// Lendo as desigualdades.
	for (i = 0; i < r; i++){
		scanf("%d%d%d%d", &x1, &y1, &x2, &y2);

		// Gravando desigualdade.
		b->cell[x1 - 1][y1 - 1]->lesser[relative_position(x1, y1, x2, y2)] = true;
		b->cell[x2 - 1][y2 - 1]->greater[relative_position(x2, y2, x1, y1)] = true;
	}

	return b;
}

void generate_possibilities(Board *b){
	int i, j, k, x;

	// Usar uma DFS para remover as possibilidades de "chains".

	// Removendo impossibilidades devido a linha e coluna.
	for (i = 0; i < b->d; i++){
		for (j = 0; j < b->d; j++){
			if (b->cell[i][j]->value){
				x = b->cell[i][j]->value;

				for (k = 0; k < b->d; k++){
					// Reduzindo o número de possibilidades da casa (i, k).
					if (!b->cell[i][k]->possibility[x]){
						b->cell[i][k]->possibility[x] = 1;
						b->cell[i][k]->n--;
					}

					// Reduzindo o número de possibilidades da casa (k, j).
					if (!b->cell[k][j]->possibility[x]){
						b->cell[k][j]->possibility[x] = 1;
						b->cell[k][j]->n--;
					}
				}
			}
		}
	}
}

void print_board(const Board *b){
	int i, j;

	// Imprimindo os valores do tabuleiro.
	for (i = 0; i < b->d; i++){
		for (j = 0; j < b->d - 1; j++){
			printf("%d ", b->cell[i][j]->value);
		}

		printf("%d\n", b->cell[i][j]->value);
	}
}

void free_board(Board *b){
	int i, j;

	// Liberando cada posição do tabuleiro.
	for (i = 0; i < b->d; i++){
		for (j = 0; j < b->d; j++){
			free_cell(b->cell[i][j]);
		}

		free(b->cell[i]);
		free(b->ref[i]);
	}

	// Liberando o tabuleiro.
	free(b->cell);
	free(b->ref);
	free(b);
}

/* --- Funções auxiliares --- */

Board *create_board(int d){
	Board *b = (Board *)malloc(sizeof(Board));
	int i, j;

	// Alocando linhas da matriz.
	b->cell = (Cell ***)malloc(d * sizeof(Cell **));
	b->ref = (PQNode ***)malloc(d * sizeof(PQNode **));
	b->d = d;

	for (i = 0; i < d; i++){
		// Alocando colunas da matriz.
		b->cell[i] = (Cell **)malloc(d * sizeof(Cell *));
		b->ref[i] = (PQNode **)calloc(d, sizeof(PQNode *));

		for (j = 0; j < d; j++){
			// Alocando elementos da matriz.
			b->cell[i][j] = create_cell(i, j, d);
		}
	}

	return b;
}

Cell *create_cell(int x, int y, int d){
	Cell *c = (Cell *)malloc(sizeof(Cell));

	// Alocando vetor de possibilidades e vetor de desigualdades.
	c->possibility = (int *)calloc(d + 1, sizeof(int));
	c->greater = (bool *)calloc(4, sizeof(bool));
	c->lesser = (bool *)calloc(4, sizeof(bool));
	c->value = 0;
	c->x = x;
	c->y = y;
	c->n = d;

	return c;
}

void free_cell(Cell *c){
	// Liberando a casa do tabuleiro.
	free(c->possibility);
	free(c->greater);
	free(c->lesser);
	free(c);
}

int relative_position(int src_x, int src_y, int dest_x, int dest_y){
	if (src_x < dest_x){
		return DOWN;
	}

	if (src_x > dest_x){
		return UP;
	}
	
	if (src_y > dest_y){
		return LEFT;
	}
	
	return RIGHT;
}