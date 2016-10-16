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
	int value; // Valor da casa do tabuleiro.
	int x, y; // Coordenadas (0 based).
	int n; // Número de possibilidades.
	int r; // Número de desigualdades envolvendo essa casa.
	int *possibility; // Vetor de possibilidades. Conta a quantidade de restrições envolvendo cada valor do domínio.
	bool *greater; // Guarda informação sobre as desigualdades.
	bool *lesser; // Guarda informação sobre as desigualdades.
};

struct Board{
	void ***ref; // Referência para os nós da Priority Queue.
	Cell ***cell; // Matriz de casas do tabuleiro.
	int d; // Dimensão do tabuleiro.
};

/* Realiza a leitura e inicialização do tabuleiro. */
Board *read_board();

/* Imprime o tabuleiro. */
void print_board(const Board *);

/* Libera o tabuleiro. */
void free_board(Board *);

#endif