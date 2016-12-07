/* Victor Forbes - 9293394 */

#include <stdlib.h>
#include <stdio.h>

#define and &&
#define or ||

/* Retorna o custo de preencher apenas X posições das L posições disponíveis. */
int cost(int x, int l){
	return (l - x) * (l - x);
}

/* Retorna o valor mínimo entre dois inteiros. */
int min(int a, int b){
	return (a < b ? a : b);
}

int main(){
	int *dp, *len, sum, i, j, l, n;
	char **s;

	// Lendo o valor de L e N.
	scanf("%d%d%*[ \n\r]", &l, &n);

	// Alocando memória para a PD e para armazenar as strings.
	dp = (int *)malloc(n * sizeof(int));
	s = (char **)malloc(n * sizeof(char *));
	len = (int *)malloc(n * sizeof(int));

	// Lendo as N strings.
	for (i = 0; i < n; i++){
		scanf("%ms%n%*[ \n\r]", s + i, len + i);
	}

	// Caso base: Custo de formar uma linha começando da string s[n - 1].
	dp[n - 1] = cost(len[n - 1], l);

	// Para cada string.
	for (i = n - 2; i >= 0; i--){
		// Inicializando com o custo de colocar essa string sozinha na linha.
		dp[i] = dp[i + 1] + cost(len[i], l);

		// Tentando adicionar as próximas strings nessa linha.
		for (j = i + 1, sum = len[i] + len[i + 1] + 1; j < n and sum <= l; j++){
			// Se estiver tentando adicionar a última string nessa linha.
			if (j == n - 1){
				dp[i] = min(dp[i], cost(sum, l));
			}
			else{
				dp[i] = min(dp[i], dp[j + 1] + cost(sum, l));
				sum += len[j + 1] + 1;
			}
		}
	}

	// Imprimindo o "caminho" ótimo.
	for (i = 0, j = 0, sum = len[0]; j < n - 1; j++){
		printf("%s", s[j]);

		// Fechando essa linha. Intervalo [i, j].
		if (dp[i] == dp[j + 1] + cost(sum, l)){
			printf("\n");

			i = j + 1;
			sum = len[i];
		}
		else{
			printf(" ");
			
			sum += len[j + 1] + 1;
		}
	}
	
	// Imprimindo a última string.
	printf("%s\n", s[j]);

	// Liberando toda memória alocada.
	for (i = 0; i < n; i++){
		free(s[i]);
	}

	free(s);
	free(dp);
	free(len);

	return 0;
}