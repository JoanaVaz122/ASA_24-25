#include <cstdio>
#include <vector>
#include <string>
#include <climits>

using namespace std;

// Função para calcular o resultado da operação binária
inline int operate(const vector<vector<int>>& table, int a, int b) {
    return table[a - 1][b - 1];
}

// Tabelas para memoização
vector<vector<vector<int>>> dp;
vector<vector<vector<string>>> parens;

// Função recursiva para resolver o problema
bool solve(const vector<int>& sequence, const vector<vector<int>>& table, int result, int start, int end) {
    // Caso já tenha sido calculado
    if (dp[start][end][result] != -1) {
        return dp[start][end][result];
    }

    // Caso base: um único número no subarray
    if (start == end) {
        if (sequence[start] == result) {
            parens[start][end][result] = to_string(sequence[start]);
            return dp[start][end][result] = true;
        }
        return dp[start][end][result] = false;
    }

    // Inicializa como falso (não encontrado)
    dp[start][end][result] = false;

    // Tenta todas as divisões possíveis
    for (int mid = start; mid < end; ++mid) {
        for (int leftResult = 1; leftResult <= table.size(); ++leftResult) {
            for (int rightResult = 1; rightResult <= table.size(); ++rightResult) {
                if (solve(sequence, table, leftResult, start, mid) &&
                    solve(sequence, table, rightResult, mid + 1, end) &&
                    operate(table, leftResult, rightResult) == result) {
                    
                    // Atualiza dp e armazena os parênteses
                    dp[start][end][result] = true;
                    parens[start][end][result] = "(" + parens[start][mid][leftResult] + " " +
                                                 parens[mid + 1][end][rightResult] + ")";
                    return true;
                }
            }
        }
    }
    return dp[start][end][result];
}

int main() {
    int n, m;
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

    // Ler o resultado alvo
    int target;
    scanf("%d", &target);

    // Inicializar tabelas de memoização
    dp.assign(m, vector<vector<int>>(m, vector<int>(n + 1, -1)));
    parens.assign(m, vector<vector<string>>(m, vector<string>(n + 1)));

    // Resolver o problema
    if (solve(sequence, table, target, 0, m - 1)) {
        printf("1\n%s\n", parens[0][m - 1][target].c_str());
    } else {
        printf("0\n");
    }

    return 0;
}
