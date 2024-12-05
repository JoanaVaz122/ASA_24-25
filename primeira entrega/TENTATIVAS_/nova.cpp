#include <vector>
#include <string>
#include <algorithm>
#include <cstdio>

using namespace std;

int operate(const vector<vector<int>>& table, int a, int b) {
    return table[a - 1][b - 1];
}

// Função recursiva para construir a expressão de parênteses
string constructParenthesis(const vector<vector<vector<string>>>& parens, int start, int end, int result) {
    if (start == end) {
        return to_string(result);
    }
    return "(" + parens[start][end][result] + ")";
}

// Função iterativa para resolver o problema de DP
bool solve(const vector<int>& sequence, const vector<vector<int>>& table, int target,
           vector<vector<vector<bool>>>& dp, vector<vector<vector<string>>>& parens) {

    // Iterando sobre todos os possíveis subproblemas
    for (int length = 1; length <= sequence.size(); ++length) {  // O tamanho do subintervalo
        for (int start = 0; start + length - 1 < sequence.size(); ++start) {  // O início do subintervalo
            int end = start + length - 1;  // O final do subintervalo

            // Iterar sobre todos os valores possíveis para o resultado
            for (size_t result = 1; result <= table.size(); ++result) {  // Usar size_t para o resultado
                // Caso base: Intervalo de um único elemento
                if (start == end) {
                    if (sequence[start] == (int)result) {  // Comparar 'sequence[start]' com 'result' convertido para int
                        dp[start][end][result] = true;
                        parens[start][end][result] = to_string(sequence[start]);
                    }
                } else {
                    // Tentar dividir o intervalo em dois subintervalos
                    for (size_t mid = start; mid < end; ++mid) {  // Usar size_t para mid
                        for (size_t leftResult = 1; leftResult <= table.size(); ++leftResult) {  // Usar size_t para leftResult
                            for (size_t rightResult = 1; rightResult <= table.size(); ++rightResult) {  // Usar size_t para rightResult
                                if (dp[start][mid][leftResult] && dp[mid + 1][end][rightResult] &&
                                    operate(table, leftResult, rightResult) == (int)result) {  // Comparar com 'result' convertido para int
                                    dp[start][end][result] = true;
                                    parens[start][end][result] = "(" + parens[start][mid][leftResult] + " " +
                                                                  parens[mid + 1][end][rightResult] + ")";
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return dp[0][sequence.size() - 1][target];
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    vector<vector<int>> table(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            scanf("%d", &table[i][j]);
        }
    }

    vector<int> sequence(m);
    for (int i = 0; i < m; ++i) {
        scanf("%d", &sequence[i]);
    }

    int target;
    scanf("%d", &target);

    // Tabelas para armazenar soluções de subproblemas
    vector<vector<vector<bool>>> dp(m, vector<vector<bool>>(m, vector<bool>(n + 1, false)));
    vector<vector<vector<string>>> parens(m, vector<vector<string>>(m, vector<string>(n + 1)));

    // Resolver o problema de forma iterativa
    if (solve(sequence, table, target, dp, parens)) {
        printf("1\n%s\n", constructParenthesis(parens, 0, m - 1, target).c_str());
    } else {
        printf("0\n");
    }

    return 0;
}
