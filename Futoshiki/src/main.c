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

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#include "utils.h"
#include "queue.h"
#include "priority_queue.h"
#include "futoshiki.h"

#define MAX_OPERATIONS 1000000 // 10^6

// Variáveis globais.
Board *b;
Queue *q;
PriorityQueue *pq;

// Vetores auxiliares para direções UP, DOWN, LEFT e RIGHT.
const int x_dir[] = {-1, 1, 0, 0};
const int y_dir[] = {0, 0, -1, 1};

// Heurísticas.
bool MRV = true;
bool FORWARD_CHECKING = true;

// Contador de atribuições feitas.
int operation_counter;

/* Função de comparação para as "variáveis" do PSR (casas do tabuleiro). */
int cell_compare(const void *a, const void *b){
	// Desempatando pela quantidade de desigualdades que envolvem a casa.
	if ((*((Cell **)a))->n == (*((Cell **)b))->n){
		return (*((Cell **)a))->r - (*((Cell **)b))->r;
	}

	// Ordenando de acordo com a quantidade de possibilidades.
	return (*((Cell **)b))->n - (*((Cell **)a))->n;
}

/* Elimina possíveis valores dado que uma casa de valor "value" é maior do que a casa "cell". */
void greater_constraint(Cell *cell, int value, int step){
	bool updated = false;
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
				updated = true;
			}
		}

		// Se tiver atualizado a quantidade de possibilidades dessa casa.
		if (updated){
			if (MRV){
				priority_queue_replace(pq, b->ref[cell->x][cell->y], &cell);
			}
			else{
				queue_replace(q, b->ref[cell->x][cell->y], &cell);
			}
		}
	}
}

/* Elimina possíveis valores dado que uma casa de valor "value" é menor do que a casa "cell". */
void lesser_constraint(Cell *cell, int value, int step){
	bool updated = false;
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
				updated = true;
			}
		}

		// Se tiver atualizado a quantidade de possibilidades dessa casa.
		if (updated){
			if (MRV){
				priority_queue_replace(pq, b->ref[cell->x][cell->y], &cell);
			}
			else{
				queue_replace(q, b->ref[cell->x][cell->y], &cell);
			}
		}
	}
}

// Elimina o valor value das possibilidades da casa cell.
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

			if (MRV){
				priority_queue_replace(pq, b->ref[cell->x][cell->y], &cell);
			}
			else{
				queue_replace(q, b->ref[cell->x][cell->y], &cell);
			}
		}
	}
}

void change_possibilities(Cell *cell, int value, int step){
	int i;

	for (i = 0; i < 4; i++){
		// Impondo restrições de >.
		if (cell->greater[i]){
			greater_constraint(b->cell[cell->x + x_dir[i]][cell->y + y_dir[i]], value, step);
		}

		// Impondo restrições de <.
		if (cell->lesser[i]){
			lesser_constraint(b->cell[cell->x + x_dir[i]][cell->y + y_dir[i]], value, step);
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

void update(Cell *cur, int value){
	// Atualizando o valor da casa.
	cur->value = value;

	// Reduzindo as possibilidades.
	change_possibilities(cur, value, 1);
}

void outdate(Cell *cur, int value){
	// Removendo o antigo valor da casa.
	cur->value = 0;

	// Aumentando as possibilidades.
	change_possibilities(cur, value, -1);
}

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

bool check(Cell *cell, int value){
	bool fc = false;
	int i;

	for (i = 0; i < 4; i++){
		// Impondo restrições de >.
		if (cell->greater[i]){
			fc = fc or check_greater(b->cell[cell->x + x_dir[i]][cell->y + y_dir[i]], value);

			if (fc){
				return true;
			}
		}

		// Impondo restrições de <.
		if (cell->lesser[i]){
			fc = fc or check_lesser(b->cell[cell->x + x_dir[i]][cell->y + y_dir[i]], value);

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

bool solve_recursively(){
	Cell *cur;
	int i;

	// If Assignment is complete.
	if ((MRV and priority_queue_empty(pq)) or (!MRV and queue_empty(q))){
		return true;
	}

	// Select Unassigned Variable.
	if (MRV){
		cur = *(Cell **)priority_queue_top(pq);
		priority_queue_pop(pq);
	}
	else{
		cur = *(Cell **)queue_front(q);
		queue_pop(q);
	}

	// For each value in Order Domain Values.
	for (i = 1; i <= b->d; i++){
		// If value is consistent with Assignment according to Constraints.
		if (!cur->possibility[i]){
			// Fazendo a verificação adiante.
			if (FORWARD_CHECKING){
				if (check(cur, i)){
					continue;
				}
			}

			// Add var = value to Assignment.
			update(cur, i);

			// Incrementando o número de atribuições.
			operation_counter++;

			// Caso o número de atribuições tenha excedido o máximo permitido.
			if (operation_counter > MAX_OPERATIONS){
				outdate(cur, i);
				printf("Numero de atribuicoes excede limite maximo\n");
				return true;
			}

			// If Recursive Backtracking == success.
			if (solve_recursively()){
				return true;
			}

			// Remove var = value from Assignment.
			outdate(cur, i);
		}
	}

	// Insert variable back to the variables container.
	if (MRV){
		b->ref[cur->x][cur->y] = priority_queue_push(pq, &cur);
	}
	else{
		b->ref[cur->x][cur->y] = queue_push(q, &cur);
	}

	return false;
}

void solve(){
	int i, j;

	operation_counter = 0;

	// Selecionando estrutura de dados de acordo com a heurística.
	if (MRV){	
		pq = priority_queue_new(sizeof(Cell *), cell_compare);

		for (i = 0; i < b->d; i++){
			for (j = 0; j < b->d; j++){
				if (!b->cell[i][j]->value){
					b->ref[i][j] = priority_queue_push(pq, b->cell[i] + j);
				}
			}
		}
	}
	else{
		q = queue_new(sizeof(Cell *));

		for (i = 0; i < b->d; i++){
			for (j = 0; j < b->d; j++){
				if (!b->cell[i][j]->value){
					b->ref[i][j] = queue_push(q, b->cell[i] + j);
				}
			}
		}
	}

	// Chamando o Backtracking recursivo.
	solve_recursively();

	if (MRV){
		priority_queue_delete(pq);
	}
	else{
		queue_delete(q);
	}
}

int main(int argc, char *argv[]){
	
	if(argc > 1){
		if(argv[1][0] < '1' or argv[1][0] > '3'){
			printf("Incorrect use of function. Usage: ./main n, wherein n is a number from 1 to 3 representing with heuristic you wish to use.\n");
			return 0;
		}
		int op = atoi(argv[1]);
		if(op == 1){
			MRV = false;
			FORWARD_CHECKING = false;
		} else if (op == 2){
			FORWARD_CHECKING = true;
			MRV = false;
		} else if(op == 3){
			MRV = true;
			FORWARD_CHECKING = true;
		} else {
			printf("Incorrect use of function. Usage: ./main n, wherein n is a number from 1 to 3 representing with heuristic you wish to use.\n");
			return 0;			
		}
	}

	clock_t start, end;
	int n, i;

	assert(scanf("%d", &n) == 1);

	for (i = 0; i < n; i++){
		start = clock();

		// Lendo o tabuleiro.
		b = read_board();

		// Solucionando.
		solve();

		end = clock();

		// Imprimindo a solução.
		printf("%.3lfs - %d)\n", (double)(end - start) / CLOCKS_PER_SEC, i + 1);
		print_board(b);
		printf("\n");

		// Liberando o tabuleiro.
		free_board(b);

	}

	return 0;
}
