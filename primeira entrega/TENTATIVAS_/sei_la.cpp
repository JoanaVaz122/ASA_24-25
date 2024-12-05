#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Função para calcular a operação binária ⊕
inline int operate(const vector<vector<int>>& table, int a, int b) {
    return table[a - 1][b - 1];
}

int main() {
    int n, m;
    cin >> n >> m;

    // Lê a tabela de operações
    vector<vector<int>> table(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> table[i][j];
        }
    }

    // Lê a sequência
    vector<int> sequence(m);
    for (int i = 0; i < m; ++i) {
        cin >> sequence[i];
    }

    // Lê o resultado desejado
    int target;
    cin >> target;

    // Matriz para guardar os resultados das operações
    vector<vector<vector<int>>> dp(m, vector<vector<int>>(m));
    vector<vector<vector<string>>> parens(m, vector<vector<string>>(m));

    // Inicializa a diagonal principal com os números da sequência
    for (int i = 0; i < m; ++i) {
        dp[i][i].push_back(sequence[i]);
        parens[i][i].push_back(to_string(sequence[i]));
    }

    // Computa as diagonais superiores
    for (int len = 2; len <= m; ++len) { // len = comprimento da subsequência
        for (int i = 0; i <= m - len; ++i) {
            int j = i + len - 1;
            // Tenta todas as posições de divisão
            for (int k = i; k < j; ++k) {
                for (int left_idx = 0; left_idx < dp[i][k].size(); ++left_idx) {
                    for (int right_idx = 0; right_idx < dp[k + 1][j].size(); ++right_idx) {
                        int left_val = dp[i][k][left_idx];
                        int right_val = dp[k + 1][j][right_idx];
                        int result = operate(table, left_val, right_val);

                        // Adiciona o resultado ao dp[i][j]
                        dp[i][j].push_back(result);
                        parens[i][j].push_back("(" + parens[i][k][left_idx] + " " + parens[k + 1][j][right_idx] + ")");
                    }
                }
            }
        }
    }

    // Verifica o resultado final no canto superior direito
    bool found = false;
    for (size_t idx = 0; idx < dp[0][m - 1].size(); ++idx) {
        if (dp[0][m - 1][idx] == target) {
            cout << "1\n" << parens[0][m - 1][idx] << endl;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "0\n";
    }

    return 0;
}
