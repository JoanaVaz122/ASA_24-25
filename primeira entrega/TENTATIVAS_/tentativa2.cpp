#include <vector>
#include <unordered_set>
#include <cstdio>
#include <string>

using namespace std;

// Operação binária na tabela
inline int operate(const vector<vector<int>>& table, int a, int b) {
    return table[a - 1][b - 1];
}

bool solveIterative(const vector<int>& sequence, const vector<vector<int>>& table, int target) {
    int m = sequence.size();
    int n = table.size();

    // Usando unordered_set para armazenar resultados válidos por intervalo
    vector<vector<unordered_set<int>>> dp(m, vector<unordered_set<int>>(m));
    vector<vector<vector<string>>> parens(m, vector<vector<string>>(m, vector<string>(n + 1)));

    // Base: Intervalos de tamanho 1
    for (int i = 0; i < m; ++i) {
        if (sequence[i] <= n) {
            dp[i][i].insert(sequence[i]);
            parens[i][i][sequence[i]] = to_string(sequence[i]);
        }
    }

    // Intervalos maiores que 1
    for (int len = 2; len <= m; ++len) {
        for (int start = 0; start <= m - len; ++start) {
            int end = start + len - 1;

            for (int mid = start; mid < end; ++mid) {
                for (int leftResult : dp[start][mid]) {
                    for (int rightResult : dp[mid + 1][end]) {
                        int result = operate(table, leftResult, rightResult);
                        if (result <= n) {
                            dp[start][end].insert(result);

                            if (parens[start][mid][leftResult].empty() || parens[mid + 1][end][rightResult].empty())
                                continue;

                            parens[start][end][result] = "(" + parens[start][mid][leftResult] + " " +
                                                         parens[mid + 1][end][rightResult] + ")";
                        }
                    }
                }
            }
        }
    }

    if (dp[0][m - 1].count(target)) {
        printf("1\n%s\n", parens[0][m - 1][target].c_str());
        return true;
    } else {
        printf("0\n");
        return false;
    }
}

int main() {
    int n, m, target;
    scanf("%d %d", &n, &m);

    // Ler a tabela de operações
    vector<vector<int>> table(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            scanf("%d", &table[i][j]);
        }
    }

    // Ler a sequência
    vector<int> sequence(m);
    for (int i = 0; i < m; ++i) {
        scanf("%d", &sequence[i]);
    }

    // Ler o alvo
    scanf("%d", &target);

    // Resolver o problema
    solveIterative(sequence, table, target);

    return 0;
}
