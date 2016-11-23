/* Victor Forbes - 9293394 */

#include <iostream>
#include <climits>

using namespace std;

#define N 1000
#define L 80

string s[N];
int dp[N];
int l, n;

int cost(int x){
	return (l - x) * (l - x);
}

int main(){
	int i, j, sum;

	ios::sync_with_stdio(false);

	cin >> l >> n;

	for (i = 0; i < n; i++){
		cin >> s[i];
	}

	// O custo de formar uma linha começando da string s[n - 1].
	dp[n - 1] = cost(s[n - 1].size());

	// Para cada string.
	for (i = n - 2; i >= 0; i--){
		// Custo de colocar essa string sozinha na linha.
		dp[i] = dp[i + 1] + cost(s[i].size());

		// Tentando adicionar as próximas strings nessa linha.
		for (j = i + 1, sum = s[i].size() + s[i + 1].size() + 1; j < n and sum <= l; j++){
			// Se não estiver tentando adicionar a última string nessa linha.
			if (j < n - 1){
				dp[i] = min(dp[i], dp[j + 1] + cost(sum));
				sum += s[j + 1].size() + 1;
			}
			else{
				dp[i] = min(dp[i], cost(sum));
			}
		}
	}

	// Imprimindo o "caminho" ótimo.
	for (i = 0, j = 0, sum = s[0].size(); j < n - 1; j++){
		cout << s[j];

		// Fechando essa linha. Intervalo [i, j].
		if (dp[i] == dp[j + 1] + cost(sum)){
			i = j + 1;
			sum = s[i].size();
			cout << endl;
		}
		else{
			cout << " ";
			sum += s[j + 1].size() + 1;
		}
	}
	
	cout << s[n - 1] << endl;

	return 0;
}