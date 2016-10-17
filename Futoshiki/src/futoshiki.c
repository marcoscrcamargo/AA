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
#include <stdio.h>
#include <assert.h>

#include "utils.h"
#include "futoshiki.h"

/* Cria um tabuleiro vazio. */
Board *create_board(int);

/* Cria uma casa do tabuleiro vazia. */
Cell *create_cell(int, int, int);

/* Libera uma casa do tabuleiro. */
void free_cell(Cell *);

/* Retorna a posição relativa entre duas casas. */
int relative_position(const Cell *, const Cell *);

/* Gera as possibilidades iniciais para cada casa do tabuleiro. */
void generate_possibilities(Board *);

/* Gera as possibilidades referentes às desigualdades. */
void check_chains(Board *);

/* Faz uma DFS no tabuleiro para achar as chains de maioridade. */
int g_dfs(Board *, int **, int, int);

/* Faz uma DFS no tabuleiro para achar as chains de menoridade. */
int l_dfs(Board *, int **, int, int);

#define X 0
#define Y 1

// Matriz auxiliar para direções UP, DOWN, LEFT e RIGHT.
const int step[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

Board *read_board(){
	int d, r, i, j, x1, y1, x2, y2;
	Cell *c1, *c2;
	Board *b;

	// Lendo a dimensão da matriz e o número de desigualdades.
	assert(scanf("%d%d", &d, &r) == 2);

	// Alocando um tabuleiro de dimensão d vazio.
	b = create_board(d);

	// Lendo os valores do tabuleiro.
	for (i = 0; i < d; i++){
		for (j = 0; j < d; j++){
			assert(scanf("%d", &b->cell[i][j]->value) == 1);
		}
	}

	// Lendo as desigualdades.
	for (i = 0; i < r; i++){
		assert(scanf("%d%d%d%d", &x1, &y1, &x2, &y2) == 4);

		// Gravando desigualdades.
		c1 = b->cell[x1 - 1][y1 - 1];
		c2 = b->cell[x2 - 1][y2 - 1];

		// c1 < c2
		c1->r++;
		c1->lesser[relative_position(c1, c2)] = true;

		// c2 > c1
		c2->r++;
		c2->greater[relative_position(c2, c1)] = true;
	}

	// Gerando as possibilidades iniciais de cada casa.
	generate_possibilities(b);

	return b;
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
	}

	// Liberando o tabuleiro.
	free(b->cell);
	free(b);
}

/* --- Funções auxiliares --- */

Board *create_board(int d){
	Board *b = (Board *)malloc(sizeof(Board));
	int i, j;

	// Alocando linhas da matriz.
	b->cell = (Cell ***)malloc(d * sizeof(Cell **));
	b->d = d;

	for (i = 0; i < d; i++){
		// Alocando colunas da matriz.
		b->cell[i] = (Cell **)malloc(d * sizeof(Cell *));

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
	c->r = 0;
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

int relative_position(const Cell *src, const Cell *dest){
	if (src->x < dest->x){
		return DOWN;
	}

	if (src->x > dest->x){
		return UP;
	}
	
	if (src->y > dest->y){
		return LEFT;
	}
	
	return RIGHT;
}

void generate_possibilities(Board *b){
	int i, j, k, x;

	// Usar uma DFS para remover as possibilidades de "chains".
	check_chains(b);

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

void check_chains(Board *b){
	int **g_chain, **l_chain;
	int i, j, k;

	// Alocando matrizes das chains greater e lesser.
	g_chain = (int **)malloc(b->d * sizeof(int *));
	l_chain = (int **)malloc(b->d * sizeof(int *));

	for (i = 0; i < b->d; i++){
		g_chain[i] = (int *)calloc(b->d, sizeof(int));
		l_chain[i] = (int *)calloc(b->d, sizeof(int));
	}

	// Rodando uma dfs para as matrizes.
	for (i = 0; i < b->d; i++){
		for (j = 0; j < b->d; j++){
			if (!g_chain[i][j]){
				g_dfs(b, g_chain, i, j);
			}

			if (!l_chain[i][j]){
				l_dfs(b, l_chain, i, j);
			}
		}
	}

	// Reduzindo as possibilidades das casas pertencentes às chains.
	for (i = 0; i < b->d; i++){
		for (j = 0; j < b->d; j++){
			for (k = 0; k < g_chain[i][j]; k++){
				b->cell[i][j]->possibility[k + 1] = 1;
				b->cell[i][j]->n--;
			}

			for (k = 0; k < l_chain[i][j]; k++){
				b->cell[i][j]->possibility[b->d - k] = 1;
				b->cell[i][j]->n--;
			}
		}
	}

	// Liberando as matrizes.
	for (i = 0; i < b->d; i++){
		free(g_chain[i]);
		free(l_chain[i]);
	}

	free(g_chain);
	free(l_chain);
}

int g_dfs(Board *b, int **g_chain, int x, int y){
	int i;

	for (i = 0; i < 4; i++){
		if (b->cell[x][y]->greater[i]){
			g_chain[x][y] = max(g_chain[x][y], g_dfs(b, g_chain, x + step[i][X], y + step[i][Y]));
		}
	}

	return g_chain[x][y] + 1;
}

int l_dfs(Board *b, int **l_chain, int x, int y){
	int i;

	for (i = 0; i < 4; i++){
		if (b->cell[x][y]->lesser[i]){
			l_chain[x][y] = max(l_chain[x][y], l_dfs(b, l_chain, x + step[i][X], y + step[i][Y]));
		}
	}

	return l_chain[x][y] + 1;
}