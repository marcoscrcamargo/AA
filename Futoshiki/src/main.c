/* Victor Forbes - 9293394 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "utils.h"
#include "queue.h"
#include "priority_queue.h"
#include "futoshiki.h"

// Variáveis globais.
Board *b;
Queue *q;
PriorityQueue *pq;

// Vetores auxiliares para direções UP, DOWN, LEFT e RIGHT.
const int x_dir[] = {-1, 1, 0, 0};
const int y_dir[] = {0, 0, -1, 1};

// Heurísticas.
bool MRV = true;
bool FORWARD_CHECKING = false;

/* Função de comparação para as "variáveis" do PSR (casas do tabuleiro). */
int cell_compare(const void *a, const void *b){
	// Desempatando pela quantidade de desigualdades que envolvem a casa.
	/*if ((*((Cell **)a))->n == (*((Cell **)b))->n){
		return (*((Cell **)a))->r - (*((Cell **)b))->r;
	}*/

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
			// Add var = value to Assignment.
			update(cur, i);

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
	clock_t start, end;
	int n, i;

	scanf("%d", &n);

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
