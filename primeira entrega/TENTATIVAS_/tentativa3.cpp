#include <vector>
#include <string>
#include <tuple>
#include <cstdio>

using namespace std;

// calcular o resultado da operação binária
int operate(const vector<vector<int>>& table, size_t a, size_t b) {
    return table[a - 1][b - 1];
}

// encontrar a parentização válida
bool solve(const vector<int>& sequence, const vector<vector<int>>& table, int target,
           vector<vector<vector<bool>>>& dp, vector<vector<vector<string>>>& parens, size_t start, size_t end) {
    if (dp[start][end][target]) {
        return true;
    }

    if (start == end) {
        if (sequence[start] == target) {
            parens[start][end][target] = to_string(sequence[start]);
            dp[start][end][target] = true;
            return true;
        }
        return false;
    }

    // iterar sobre as divisões da direita para a esquerda
    for (size_t mid = start; mid < end; ++mid) {
        for (size_t leftResult = 1; leftResult <= table.size(); ++leftResult) {
            for (size_t rightResult = 1; rightResult <= table.size(); ++rightResult) {
                if (solve(sequence, table, leftResult, dp, parens, start, mid) &&
                    solve(sequence, table, rightResult, dp, parens, mid + 1, end) &&
                    operate(table, leftResult, rightResult) == target) {
                    
                    dp[start][end][target] = true;

                    // mete os parênteses com prioridade nas operações da direita
                    parens[start][end][target] = "(" + parens[start][mid][leftResult] + " " +
                                                 parens[mid + 1][end][rightResult] + ")";
                    return true;
                }
            }
        }
    }
    return false;
}

int main() {
    size_t n, m;
    scanf("%zu %zu", &n, &m);

    vector<vector<int>> table(n, vector<int>(n));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            scanf("%d", &table[i][j]);
        }
    }

    vector<int> sequence(m);
    for (size_t i = 0; i < m; ++i) {
        scanf("%d", &sequence[i]);
    }

    int target;  // target é agora int para correspondência com sequence
    scanf("%d", &target);

    // tabelas 
    vector<vector<vector<bool>>> dp(m, vector<vector<bool>>(m, vector<bool>(n + 1, false)));
    vector<vector<vector<string>>> parens(m, vector<vector<string>>(m, vector<string>(n + 1)));

    // resolver o problema
    if (solve(sequence, table, target, dp, parens, 0, m - 1)) {
        printf("1\n%s\n", parens[0][m - 1][target].c_str());
    } else {
        printf("0\n");
    }

    return 0;
}
