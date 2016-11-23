/* Victor Forbes - 9293394 */

#include <iostream>

using namespace std;

#define N 1000
#define L 80

string s[N];
int dp[N][L];
int l, n;

int cost(int x){
	return (l - x) * (l - x);
}

int main(){
	int i, j;

	ios::sync_with_stdio(false);

	cin >> l >> n;

	for (i = 0; i < n; i++){
		cin >> s[i];
	}

	// Preenchendo a última linha da matriz da PD (caso base).
	for (j = 0; j < l; j++){
		// Se der para colocar a string s[n - 1] na posição j.
		if (j + (int)s[n - 1].size() <= l){
			// Custo de preencher a posição j com a string s[n - 1].
			dp[n - 1][j] = cost(j + s[n - 1].size());
		}
		else{
			// Impossível colocar a string na posição j.
			dp[n - 1][j] = -1;
		}
	}

	// Para cada string.
	for (i = n - 2; i >= 0; i--){
		// Para cada posição.
		for (j = 0; j < l; j++){
			// Se der para colocar a string s[i] na posição j.
			if (j + (int)s[i].size() <= l){
				// Se acabou a linha ao colocar a string s[i] ou se a string s[i + 1] precisar começar da posição 0.
				if (j + (int)s[i].size() >= l - 1 or dp[i + 1][j + s[i].size() + 1] == -1){
					// Pulando a linha.
					dp[i][j] = dp[i + 1][0] + cost(j + s[i].size());
				}
				else{
					// Escolhendo entre pular a linha e se manter na mesma linha.
					dp[i][j] = min(dp[i + 1][0] + cost(j + s[i].size()), dp[i + 1][j + s[i].size() + 1]);
				}
			}
			else{
				// Impossível colocar a string na posição j.
				dp[i][j] = -1;
			}
		}
	}

	for (i = 0; i < n; i++){
		for (j = 0; j < l; j++){
			cout << dp[i][j] << " ";
		}

		cout << endl;
	}

	// Imprimindo o "caminho" ótimo.
	for (i = 0, j = 0; i < n - 1; i++){
		// Se a escolha ótima for pular de linha.
		if (dp[i][j] == cost(j + s[i].size()) + dp[i + 1][0]){
			cout << s[i] << endl;

			j = 0;
		}
		else{
			cout << s[i] << " ";

			j += s[i].size() + 1;
		}
	}
	
	cout << s[i] << endl;

	return 0;
}