#include <vector>
#include <string>
#include <cstdio>
#include <iostream>

using namespace std;

// Função para calcular a operação binária ⊕
inline int operate(const vector<vector<int>>& table, int a, int b) {
    return table[a - 1][b - 1];
}

// Função para preencher o "quadrado" na matriz
void fill_square(int lin, int col, vector<vector<vector<int>>>& tabela, 
                 const vector<vector<int>>& matriz, int primeiro, int ultimo, int n, int v) {
    int full = 0;
    for (int k = lin; k < col; ++k) {
        for (int i = 0; i < n; ++i) {
            if (tabela[lin][k][i] == -1) break; // Valor inválido, interrompe
            for (int j = 0; j < n; ++j) {
                if (tabela[k + 1][col][j] == -1) break; // Valor inválido, interrompe
                int res = matriz[i][j]; // Calcula o resultado da operação

                if (ultimo && res == v) {
                    tabela[lin][col][0] = res; // Armazena o resultado final
                    tabela[lin][col][res] = k; // Ponto de divisão
                    tabela[lin][col][res + n] = i; // Índice do lado esquerdo
                    tabela[lin][col][res + (2 * n)] = j; // Índice do lado direito
                    return;
                } else if (tabela[lin][col][res] == -1) {
                    tabela[lin][col][full] = res; 
                    tabela[lin][col][res] = k; 
                    tabela[lin][col][res + n] = i;
                    tabela[lin][col][res + (2 * n)] = j;
                    full++;
                    if (full == 0) return; // Espaço esgotado, interrompe
                }
            }
        }
    }
}

string reconstruct(int start, int end, int target, const vector<vector<vector<int>>>& tabela, const vector<int>& sequence) {
    if (start == end) return to_string(sequence[start]);
    
    int k = tabela[start][end][target];
    int leftResult = tabela[start][k][target + sequence.size()];
    int rightResult = tabela[k + 1][end][target + 2 * sequence.size()];
    string left = reconstruct(start, k, leftResult, tabela, sequence);
    string right = reconstruct(k + 1, end, rightResult, tabela, sequence);
    
    return "(" + left + " " + right + ")";
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    // Lê a tabela de operações
    vector<vector<int>> matriz(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            scanf("%d", &matriz[i][j]);
        }
    }

    // Lê a sequência
    vector<int> sequence(m);
    for (int i = 0; i < m; ++i) {
        scanf("%d", &sequence[i]);
    }

    // Lê o valor alvo
    int v;
    scanf("%d", &v);

    // Inicializa a tabela
    vector<vector<vector<int>>> tabela(m, vector<vector<int>>(m, vector<int>(n * 3, -1)));

    // Preenche a diagonal principal
    for (int i = 0; i < m; ++i) {
        tabela[i][i][0] = sequence[i];
    }

    // Preenche as diagonais superiores
    for (int len = 2; len <= m; ++len) {
        for (int i = 0; i <= m - len; ++i) {
            int j = i + len - 1;
            fill_square(i, j, tabela, matriz, i, (len == m), n, v);
        }
    }

    // Verifica o resultado final
    if (tabela[0][m - 1][0] == v) {
        printf("1\n");
        // Reconstrói os parênteses a partir da tabela
        string result = reconstruct(0, m - 1, v, tabela, sequence);
        cout << result << endl;
    } else {
        printf("0\n");
    }

    return 0;
}