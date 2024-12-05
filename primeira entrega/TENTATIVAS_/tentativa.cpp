#include <vector>
#include <string>
#include <iostream>

using namespace std;

// Função para calcular o resultado de uma operação binária
inline int operate(const vector<vector<int>>& table, int a, int b) {
    return table[a - 1][b - 1];
}

// Função para resolver o problema de forma iterativa
bool solveIterative(const vector<int>& sequence, const vector<vector<int>>& table, int target) {
    int m = sequence.size();  // Tamanho da sequência
    int n = table.size();     // Número de elementos possíveis (tabela)

    // DP para armazenar se é possível obter um resultado em um intervalo específico
    vector<vector<vector<bool>>> dp(m, vector<vector<bool>>(m, vector<bool>(n + 1, false)));

    // Parênteses para reconstruir a expressão
    vector<vector<vector<string>>> parens(m, vector<vector<string>>(m, vector<string>(n + 1)));

    // Base: Intervalos de tamanho 1 (cada elemento individual da sequência)
    for (int i = 0; i < m; ++i) {
        if (sequence[i] <= n) {
            dp[i][i][sequence[i]] = true;
            parens[i][i][sequence[i]] = to_string(sequence[i]);
        }
    }

    // Intervalos maiores que 1
    for (int len = 2; len <= m; ++len) {  // Comprimento do intervalo
        for (int start = 0; start <= m - len; ++start) {
            int end = start + len - 1;  // Final do intervalo

            // Iterar sobre os resultados possíveis
            for (int result = 1; result <= n; ++result) {
                for (int mid = start; mid < end; ++mid) {  // Divisão do intervalo
                    for (int leftResult = 1; leftResult <= n; ++leftResult) {
                        for (int rightResult = 1; rightResult <= n; ++rightResult) {
                            if (dp[start][mid][leftResult] && dp[mid + 1][end][rightResult] &&
                                operate(table, leftResult, rightResult) == result) {
                                
                                dp[start][end][result] = true;
                                parens[start][end][result] = "(" + parens[start][mid][leftResult] +
                                                             " " + parens[mid + 1][end][rightResult] + ")";
                            }
                        }
                    }
                }
            }
        }
    }

    // Verificar se o alvo pode ser construído a partir do intervalo completo
    if (dp[0][m - 1][target]) {
        cout << "1\n" << parens[0][m - 1][target] << "\n";
        return true;
    } else {
        cout << "0\n";
        return false;
    }
}

int main() {
    int n, m;
    cin >> n >> m;

    // Ler a tabela de operações
    vector<vector<int>> table(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> table[i][j];
        }
    }

    // Ler a sequência
    vector<int> sequence(m);
    for (int i = 0; i < m; ++i) {
        cin >> sequence[i];
    }

    // Ler o alvo
    int target;
    cin >> target;

    // Resolver o problema
    solveIterative(sequence, table, target);

    return 0;
}
