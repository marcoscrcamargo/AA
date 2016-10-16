/* Victor Forbes - 9293394 */

#include <stdlib.h>
#include <stdio.h>

#include "utils.h"
#include "queue.h"
#include "priority_queue.h"
#include "futoshiki.h"

Board *b;
Queue *q;
PriorityQueue *pq;

// Vetores auxiliares para direções UP, DOWN, LEFT e RIGHT.
int x_dir[] = {-1, 1, 0, 0};
int y_dir[] = {0, 0, -1, 1};

void greater_constraint(Cell *cell, int value, int step){
	int i, prev, cur;

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

			priority_queue_replace(pq, b->ref[cell->x][cell->y], &cell);
		}
	}
}

void lesser_constraint(Cell *cell, int value, int step){
	int i, prev, cur;

	if (!cell->value){
		// Para todos os valores maiores do que value.
		for (i = value - 1; i >= 1; i--){
			// Removendo i das possibilidades.
			prev = cell->possibility[i];
			cell->possibility[i] += step;
			cur = cell->possibility[i];

			// Se foi de 0 para 1 ou 1 para 0.
			if ((prev and !cur) or (!prev and cur)){
				cell->n -= step;
			}

			priority_queue_replace(pq, b->ref[cell->x][cell->y], &cell);
		}
	}
}

void equal_constraint(Cell *cell, int value, int step){
	int prev, cur;

	if (!cell->value){
		prev = cell->possibility[value];
		cell->possibility[value] += step;
		cur = cell->possibility[value];

		// Se foi de 0 para 1 ou 1 para 0.
		if ((prev and !cur) or (!prev and cur)){
			cell->n -= step;
		}

		priority_queue_replace(pq, b->ref[cell->x][cell->y], &cell);
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
	cur->value = value;

	// Reduzindo as possibilidades.
	change_possibilities(cur, value, 1);
}

void outdate(Cell *cur, int value){
	cur->value = 0;

	// Aumentando as possibilidades.
	change_possibilities(cur, value, -1);
}

int cell_compare(const void *a, const void *b){
	return (*((Cell **)b))->n - (*((Cell **)a))->n;
}

bool solve_recursively(){
	Cell *cur;
	int i;

	if (priority_queue_empty(pq)){
		return true;
	}

	cur = *(Cell **)priority_queue_top(pq);
	priority_queue_pop(pq);

	// print_board(b);
	// printf("\n");

	// printf("Var = (%d, %d) com %d possibilidades\n", cur->x, cur->y, cur->n);

	for (i = 1; i <= b->d; i++){
		if (!cur->possibility[i]){
			update(cur, i);

			if (solve_recursively()){
				return true;
			}

			outdate(cur, i);
		}
	}

	b->ref[cur->x][cur->y] = priority_queue_push(pq, &cur);

	return false;
}

void solve(){
	int i, j;

	pq = priority_queue_new(sizeof(Cell *), cell_compare);

	for (i = 0; i < b->d; i++){
		for (j = 0; j < b->d; j++){
			if (!b->cell[i][j]->value){
				b->ref[i][j] = priority_queue_push(pq, b->cell[i] + j);
			}
		}
	}

	solve_recursively();

	priority_queue_delete(pq);
}

int main(int argc, char *argv[]){
	int n, i;

	scanf("%d", &n);

	for (i = 0; i < n; i++){
		// Lendo o tabuleiro.
		b = read_board();

		// Gerando as possibilidades iniciais de cada casa.
		generate_possibilities(b);

		// Solucionando.
		solve();

		// Imprimindo a solução.
		printf("%d)\n", i + 1);
		print_board(b);
		printf("\n");

		// Liberando o tabuleiro.
		free_board(b);
	}

	return 0;
}
