/* Victor Forbes - 9293394 */

#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

/* Retorna o custo de preencher apenas X posições das L posições disponíveis. */
int cost(int x, int l){
	return (l - x) * (l - x);
}

int main(){
	int **dp, *len, i, j, l, n;
	char **s;

	// Lendo o valor de L e N.
	scanf("%d%d%*c", &l, &n);

	// Alocando memória para a PD e para armazenar as strings.
	dp = (int **)malloc_matrix(n, l, sizeof(int));
	s = (char **)malloc(n * sizeof(char *));
	len = (int *)malloc(n * sizeof(int));

	// Lendo as N strings.
	for (i = 0; i < n; i++){
		scanf("%ms%n%*c", s + i, len + i);
	}

	// Preenchendo a última linha da matriz da PD (caso base).
	for (j = 0; j < l; j++){
		// Se der para colocar a string s[n - 1] na posição j.
		if (j + len[n - 1] <= l){
			// Custo de preencher a posição j com a string s[n - 1].
			dp[n - 1][j] = cost(j + len[n - 1], l);
		}
		else{
			// Impossível colocar a string s[n - 1] na posição j.
			dp[n - 1][j] = -1;
		}
	}

	// Para cada string.
	for (i = n - 2; i >= 0; i--){
		// Para cada posição.
		for (j = 0; j < l; j++){
			// Se der para colocar a string s[i] na posição j.
			if (j + len[i] <= l){
				// Se acabou a linha ao colocar a string s[i] ou se a string s[i + 1] precisar começar da posição 0.
				if (j + len[i] >= l - 1 or dp[i + 1][j + len[i] + 1] == -1){
					// Pulando a linha.
					dp[i][j] = dp[i + 1][0] + cost(j + len[i], l);
				}
				else{
					// Escolhendo entre pular a linha e se manter na mesma linha.
					dp[i][j] = min(dp[i + 1][0] + cost(j + len[i], l), dp[i + 1][j + len[i] + 1]);
				}
			}
			else{
				// Impossível colocar a string s[i] na posição j.
				dp[i][j] = -1;
			}
		}
	}

	// Imprimindo o "caminho" ótimo.
	for (i = 0, j = 0; i < n - 1; i++){
		// Se a escolha ótima for pular de linha.
		if (dp[i][j] == cost(j + len[i], l) + dp[i + 1][0]){
			printf("%s\n", s[i]);

			j = 0;
		}
		else{
			printf("%s ", s[i]);

			j += len[i] + 1;
		}
	}

	printf("%s\n", s[i]);

	// Liberando toda memória alocada.
	free_matrix((void **)dp, n);
	free_matrix((void **)s, n);
	free(len);

	return 0;
}