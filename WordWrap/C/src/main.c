/* Marcos Cesar Ribeiro de Camargo (9278045), Victor Luiz Roquete Forbes (9293394)

This file is part of Word Wrap.

Word Wrap is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Word Wrap is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Word Wrap.  If not, see <http://www.gnu.org/licenses/>. */

#include <stdlib.h>
#include <stdio.h>

#define and &&
#define or ||

/* Retorna o valor mínimo entre dois inteiros. */
int min(int a, int b){
	return (a < b ? a : b);
}

/* Retorna o custo de preencher uma linha de tamanho L com X caracteres. */
int cost(int x, int l){
	return (l - x) * (l - x);
}

/* Retorna o vetor de memoização com a resposta ótima para cada string s[i]. */
int *solve(char **s, const int *len, int l, int n){
	int *dp, sum, i, j;

	// Alocando memória para o vetor de memoização.
	dp = (int *)malloc(n * sizeof(int));

	// Preenchendo o caso base com o custo de formar uma linha começando da string s[n - 1].
	dp[n - 1] = cost(len[n - 1], l);

	// Para cada string.
	for (i = n - 2; i >= 0; i--){
		// Inicializando com o custo de colocar essa string sozinha na linha.
		dp[i] = dp[i + 1] + cost(len[i], l);

		// Tentando adicionar as próximas strings nessa linha.
		for (j = i + 1, sum = len[i] + len[i + 1] + 1; j < n and sum <= l; j++){
			// Se estiver tentando adicionar a última string nessa linha.
			if (j == n - 1){
				// Tentando colocar todas as strings a partir da string s[i] na mesma linha.
				dp[i] = min(dp[i], cost(sum, l));
			}
			else{
				// Tentando colocar todas as strings de s[i] até s[j] na mesma linha.
				dp[i] = min(dp[i], dp[j + 1] + cost(sum, l));
				sum += len[j + 1] + 1;
			}
		}
	}

	return dp;
}

/* Imprime a resposta ótima. */
void print(int *dp, char **s, int *len, int l, int n){
	int sum, i, j;

	// Para cada intervalo [i..j].
	for (i = 0, j = 0, sum = len[0]; j < n - 1; j++){
		// Imprimindo a string s[j].
		printf("%s", s[j]);

		// Se a decisão ótima for "fechar" a linha com as strings de s[i] até s[j].
		if (dp[i] == dp[j + 1] + cost(sum, l)){
			printf("\n");

			// Começando uma nova linha com a próxima string.
			i = j + 1;
			sum = len[i];
		}
		else{
			printf(" ");
			
			// Somando a quantidade de caracteres.
			sum += len[j + 1] + 1;
		}
	}
	
	// Imprimindo a última string.
	printf("%s\n", s[j]);
}

int main(int argc, char *argv[]){
	int *dp, *len, i, l, n;
	char **s;

	// Lendo o valor de L e N.
	scanf("%d%d%*[ \n\r]", &l, &n);

	// Alocando memória para armazenar as strings e seus tamanhos.
	s = (char **)malloc(n * sizeof(char *));
	len = (int *)malloc(n * sizeof(int));

	// Lendo as N strings.
	for (i = 0; i < n; i++){
		scanf("%ms%n%*[ \n\r]", s + i, len + i);
	}

	// Preenchendo o vetor de memoização.
	dp = solve(s, len, l, n);

	for (i = 0; i < n; i++){
		printf("%d ", dp[i]);
	}

	printf("\n");

	// Imprimindo a solução ótima.
	print(dp, s, len, l, n);

	// Liberando toda memória alocada.
	for (i = 0; i < n; i++){
		free(s[i]);
	}

	free(s);
	free(dp);
	free(len);

	return 0;
}