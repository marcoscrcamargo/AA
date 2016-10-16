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