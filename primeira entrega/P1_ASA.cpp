#include <vector>
#include <string>
#include <iostream>
#include <tuple>

using namespace std;

// calcular o resultado da operação binaria 
int operate(const vector<vector<int>>& table, int a, int b) {
    return table[a - 1][b - 1];
}

// encontrar a parentizacao valida
bool solve(const vector<int>& sequence, const vector<vector<int>>& table, int result,
           vector<vector<vector<bool>>>& dp, vector<vector<vector<string>>>& parens, int start, int end) {
    if (dp[start][end][result]) {
        return true;
    }

    if (start == end) {
        if (sequence[start] == result) {
            parens[start][end][result] = to_string(sequence[start]);
            dp[start][end][result] = true;
            return true;
        }
        return false;
    }

    // iterar sobre as divisoes da direita para a esquerda
    for (int mid = end - 1; mid >= start; --mid) {
        for (size_t leftResult = 1; leftResult <= table.size(); ++leftResult) {
            for (size_t rightResult = 1; rightResult <= table.size(); ++rightResult) {
                if (solve(sequence, table, leftResult, dp, parens, start, mid) &&
                    solve(sequence, table, rightResult, dp, parens, mid + 1, end) &&
                    operate(table, leftResult, rightResult) == result) {
                    
                    dp[start][end][result] = true;

                    // mete os parenteses com prioridade nas operacoes da direita
                    parens[start][end][result] = "(" + parens[start][mid][leftResult] + " " +
                                                 parens[mid + 1][end][rightResult] + ")";
                    return true;
                }
            }
        }
    }
    return false;
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
