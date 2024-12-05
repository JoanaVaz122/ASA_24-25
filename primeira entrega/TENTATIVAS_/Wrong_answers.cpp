#include <vector>
#include <string>
#include <cstdio>
using namespace std;

// Calcula o resultado da operação binária
inline int operate(const vector<vector<int>>& table, int a, int b) {
    return table[a - 1][b - 1];
}

// Solução com memoização eficiente e lógica corrigida
bool solve(const vector<int>& sequence, const vector<vector<int>>& table, int result,
           vector<vector<vector<bool>>>& computed, vector<vector<vector<bool>>>& dp,
           vector<vector<vector<string>>>& parens, int start, int end) {
    if (result < 1 || result > (int)table.size()) {
        return false; // Resultado fora do intervalo válido
    }

    if (computed[start][end][result]) {
        return dp[start][end][result];
    }

    computed[start][end][result] = true ;

    // Caso base: único número na sequência
    if (start == end) {
        if (sequence[start] == result) {
            parens[start][end][result] = to_string(sequence[start]);
            return dp[start][end][result] = true;
        }
        return dp[start][end][result] = false;
    }

    // Dividir e conquistar para verificar todas as combinações
    for (int mid = start; mid < end; ++mid) {
        for (size_t leftResult = 1; leftResult <= table.size(); ++leftResult) {
            for (size_t rightResult = 1; rightResult <= table.size(); ++rightResult) {
                if (solve(sequence, table, leftResult, computed, dp, parens, start, mid) &&
                    solve(sequence, table, rightResult, computed, dp, parens, mid + 1, end)) {

                    int operationResult = operate(table, leftResult, rightResult);
                    if (operationResult == result) {
                        dp[start][end][result] = true;
                        parens[start][end][result] = "(" + parens[start][mid][leftResult] + " " +
                                                     parens[mid + 1][end][rightResult] + ")";
                        return true;
                    }
                }
            }
        }
    }

    return dp[start][end][result] = false;
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

    // Estruturas para memoização
    vector<vector<vector<bool>>> computed(m, vector<vector<bool>>(m, vector<bool>(n + 1, false)));
    vector<vector<vector<bool>>> dp(m, vector<vector<bool>>(m, vector<bool>(n + 1, false)));
    vector<vector<vector<string>>> parens(m, vector<vector<string>>(m, vector<string>(n + 1)));

    // Resolver o problema
    if (solve(sequence, table, target, computed, dp, parens, 0, m - 1)) {
        printf("1\n%s\n", parens[0][m - 1][target].c_str());
    } else {
        printf("0\n");
    }

    return 0;
}