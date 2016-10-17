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
#include <time.h>

#include "utils.h"
#include "futoshiki.h"

#define MAX_OPERATIONS 1000000 // 10^6

// Defines auxiliares para as direções.
#define X 0
#define Y 1

// Matriz auxiliar para direções UP, DOWN, LEFT e RIGHT.
const int dir[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

// Variáveis globais.
Board *b;

// Heurísticas.
bool MRV = true;
bool FORWARD_CHECKING = true;

// Contador de atribuições feitas.
int operation_counter;

/* Função de comparação para as "variáveis" do PSR (casas do tabuleiro). */
int cell_compare(const void *a, const void *b){
	// Desempatando pela quantidade de desigualdades que envolvem a casa.
	if ((*((Cell **)a))->n == (*((Cell **)b))->n){
		// Desempatando pela coordenada x.
		if ((*((Cell **)a))->r == (*((Cell **)b))->r){
			// Desempatando pela coordenada y.
			if ((*((Cell **)a))->x == (*((Cell **)b))->x){
				return (*((Cell **)a))->y - (*((Cell **)b))->y;
			}

			return (*((Cell **)a))->x - (*((Cell **)b))->x;
		}

		return (*((Cell **)b))->r - (*((Cell **)a))->r;
	}

	// Ordenando de acordo com a quantidade de possibilidades.
	return (*((Cell **)a))->n - (*((Cell **)b))->n;
}

/* O(D) - Elimina possíveis valores dado que uma casa de valor "value" é maior do que a casa "cell". */
void greater_constraint(Cell *cell, int value, int step){
	int i, prev, cur;

	// Se for uma casa vazia.
	if (!cell->value){
		// Para todos os valores maiores do que value.
		for (i = value + 1; i <= b->d; i++){
			// Removendo i das possibilidades.
			prev = cell->possibility[i];
			cell->possibility[i] += step;
			cur = cell->possibility[i];

			// Se foi de 0 para 1 ou 1 para 0.
			if ((prev and !cur) or (!prev and cur)){
				cell->n -= step;
			}
		}
	}
}

/* O(D) - Elimina possíveis valores dado que uma casa de valor "value" é menor do que a casa "cell". */
void lesser_constraint(Cell *cell, int value, int step){
	int i, prev, cur;

	// Se for uma casa vazia.
	if (!cell->value){
		// Para todos os valores menores do que value.
		for (i = value - 1; i >= 1; i--){
			// Removendo i das possibilidades.
			prev = cell->possibility[i];
			cell->possibility[i] += step;
			cur = cell->possibility[i];

			// Se foi de 0 para 1 ou 1 para 0.
			if ((prev and !cur) or (!prev and cur)){
				cell->n -= step;
			}
		}
	}
}

/* O(1) - Elimina o valor value das possibilidades da casa cell. */
void equal_constraint(Cell *cell, int value, int step){
	int prev, cur;

	// Se for uma casa vazia.
	if (!cell->value){
		prev = cell->possibility[value];
		cell->possibility[value] += step;
		cur = cell->possibility[value];

		// Se foi de 0 para 1 ou 1 para 0.
		if ((prev and !cur) or (!prev and cur)){
			cell->n -= step;
		}
	}
}

/* O(D) - Muda as possibilidades dos vértices adjacentes. */
void change_possibilities(Cell *cell, int value, int step){
	int i;

	// Impondo restrições de desigualdades.
	for (i = 0; i < 4; i++){
		// Impondo restrições de >.
		if (cell->greater[i]){
			greater_constraint(b->cell[cell->x + dir[i][X]][cell->y + dir[i][Y]], value, step);
		}

		// Impondo restrições de <.
		if (cell->lesser[i]){
			lesser_constraint(b->cell[cell->x + dir[i][X]][cell->y + dir[i][Y]], value, step);
		}
	}

	// Impondo restrições estilo Sudoku.
	for (i = 0; i < b->d; i++){
		if (i != cell->y){
			equal_constraint(b->cell[cell->x][i], value, step);
		}

		if (i != cell->x){
			equal_constraint(b->cell[i][cell->y], value, step);
		}
	}
}

/* O(D) - Atualiza as possibilidades dos vértices adjacentes. */
void update(Cell *cur, int value){
	// Atualizando o valor da casa.
	cur->value = value;

	// Reduzindo as possibilidades.
	change_possibilities(cur, value, 1);
}

/* O(D) - Desatualiza as possibilidades dos vértices adjacentes. */
void outdate(Cell *cur, int value){
	// Removendo o antigo valor da casa.
	cur->value = 0;

	// Aumentando as possibilidades.
	change_possibilities(cur, value, -1);
}

/* O(D) - Função que verifica se uma casa ficará sem possibilidades pelo critério da maioridade. */
bool check_greater(Cell *cell, int value){
	int i, n;

	// Se for uma casa vazia.
	if (!cell->value){
		n = cell->n;

		// Para todos os valores maiores do que value.
		for (i = value; i <= b->d; i++){
			// Removendo i das possibilidades.
			if (cell->possibility[i] == 0){
				n--;
			}
		}

		return n == 0;
	}

	return false;
}

/* O(D) - Função que verifica se uma casa ficará sem possibilidades pelo critério da menoridade. */
bool check_lesser(Cell *cell, int value){
	int i, n;

	// Se for uma casa vazia.
	if (!cell->value){
		n = cell->n;

		// Para todos os valores maiores do que value.
		for (i = value; i >= 1; i--){
			// Removendo i das possibilidades.
			if (cell->possibility[i] == 0){
				n--;
			}
		}

		return n == 0;
	}

	return false;
}

/* O(1) - Função que verifica se uma casa ficará sem possibilidades pelo critério Sudoku. */
bool check_equal(Cell *cell, int value){
	int n;

	// Se for uma casa vazia.
	if (!cell->value){
		n = cell->n;

		if (cell->possibility[value] == 0){
			n--;
		}

		return n == 0;
	}

	return false;
}

/* O(D) - Função que verifica se alguma casa vazia ficará sem possibilidades de atribuição. */
bool check(Cell *cell, int value){
	bool fc = false;
	int i;

	for (i = 0; i < 4; i++){
		// Impondo restrições de >.
		if (cell->greater[i]){
			fc = fc or check_greater(b->cell[cell->x + dir[i][X]][cell->y + dir[i][Y]], value);

			if (fc){
				return true;
			}
		}

		// Impondo restrições de <.
		if (cell->lesser[i]){
			fc = fc or check_lesser(b->cell[cell->x + dir[i][X]][cell->y + dir[i][Y]], value);

			if (fc){
				return true;
			}
		}
	}

	// Impondo restrições estilo Sudoku.
	for (i = 0; i < b->d; i++){
		if (i != cell->y){
			fc = fc or check_equal(b->cell[cell->x][i], value);

			if (fc){
				return true;
			}
		}

		if (i != cell->x){
			fc = fc or check_equal(b->cell[i][cell->y], value);

			if (fc){
				return true;
			}
		}
	}

	return false;
}

/* O(D^2) - Função que seleciona uma casa vazia para ser preenchida. */
Cell *select_unassigned_variable(){
	Cell *cell = NULL;
	int i, j;

	// Buscando alguma casa vazia.
	for (i = 0; i < b->d; i++){
		for (j = 0; j < b->d; j++){
			if (!b->cell[i][j]->value){
				if (MRV){
					// Atualizando a casa vazia de mínimo número de possibilidades.
					if (!cell or cell_compare(b->cell[i] + j, &cell) < 0){
						cell = b->cell[i][j];
					}
				}
				else{
					// Retornando a primeira casa vazia.
					return b->cell[i][j];
				}
			}
		}
	}

	return cell;
}

/* O(D^2) - Função recursiva que realiza o backtracking. */
bool solve(){
	Cell *cur;
	int i;

	// O(D^2) - Select Unassigned Variable.
	cur = select_unassigned_variable();

	// O(1) - If Assignment is complete.
	if (!cur){
		return true;
	}

	// O(D) - For each value in Order Domain Values.
	for (i = 1; i <= b->d; i++){
		// O(1) - If value is consistent with Assignment according to Constraints.
		if (!cur->possibility[i]){
			// O(D) - Rodando o Forward Checking
			if (FORWARD_CHECKING and check(cur, i)){
				continue;
			}

			// O(D) - Add var = value to Assignment.
			update(cur, i);

			// O(1) - Incrementando o número de atribuições.
			operation_counter++;

			// O(1) - Caso o número de atribuições tenha excedido o máximo permitido.
			if (operation_counter > MAX_OPERATIONS){
				outdate(cur, i);
				printf("Numero de atribuicoes excede limite maximo\n");
				return true;
			}

			// If Recursive Backtracking == success.
			if (solve()){
				return true;
			}

			// O(D) - Remove var = value from Assignment.
			outdate(cur, i);
		}
	}

	return false;
}

int main(int argc, char *argv[]){
	clock_t start, end;
	int n, i;

	/* Verificando argumentos passados pelo terminal. */
	if (argc > 1){
		if (argv[1][0] < '1' or argv[1][0] > '3'){
			printf("Incorrect use of function. Usage: ./main n, where n is a number from 1 to 3 representing which heuristic you wish to use.\n");
			return 0;
		}

		switch (argv[1][0]){
			case '1':
				MRV = false;
				FORWARD_CHECKING = false;
				break;
			case '2':
				FORWARD_CHECKING = true;
				MRV = false;
				break;
			case '3':
				MRV = true;
				FORWARD_CHECKING = true;
				break;
			default:
				break;
		}
	}

	assert(scanf("%d", &n) == 1);

	for (i = 0; i < n; i++){
		start = clock();

		// Lendo o tabuleiro.
		b = read_board();

		operation_counter = 0;

		// Solucionando.
		solve();

		end = clock();

		// Imprimindo a solução.
		printf("%d / %.3lfs - %d)\n", operation_counter, (double)(end - start) / CLOCKS_PER_SEC, i + 1);
		print_board(b);
		printf("\n");

		// Liberando o tabuleiro.
		free_board(b);
	}

	return 0;
}
