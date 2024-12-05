#include <vector>
#include <string>
#include <cstdio>
#include <algorithm>
#include <limits>

using namespace std;

// Preencher a matriz com resultados
void fill_square(int lin, int col, vector<vector<vector<int>>>& tabela,
                 const vector<vector<int>>& matriz, int v, int n, int ultimo, int primeiro) {
    if (lin >= col || lin < 0 || col >= (int)tabela.size()) return; // Verificacao de limites

    if (tabela[lin][col].size() < (size_t)(n * 3)) {
        tabela[lin][col].resize(n * 3, -1); // Redimensiona para suportar todos os estados
        tabela[col][lin].resize(n * 3, -1);
    }

    int full = 0;
    bool found = false;
    for (int k = col - 1; k >= lin; k--) { // Tenta todas as divisoes
        for (int i : tabela[lin][k]) { // Valores a esquerda
            if (i == -1) break;
            for (int j : tabela[k + 1][col]) { // Valores à direita
                if (j == -1) break;

                int res = matriz[i][j] - 1;
                if (res < 0 || res >= n) continue; // Valores inválidos

                if (full < n && tabela[col][lin][res] == -1) {
                    tabela[lin][col][full] = res;   // Armazena o resultado
                    tabela[col][lin][res] = k;     // Índice da divisao
                    tabela[col][lin][res + n] = i; // Operando esquerdo
                    tabela[col][lin][res + 2 * n] = j; // Operando direito
                    full++;
                    found = true;
                }

                if (ultimo && res == v) {
                    tabela[lin][col][0] = res;
                    tabela[col][lin][res] = k;
                    tabela[col][lin][res + n] = i;
                    tabela[col][lin][res + 2 * n] = j;
                    return; // Para assim que encontrar
                }

                // Se já encontrou todos os resultados possíveis, para os loops
                if (full >= n) {
                    found = true;
                    break;
                }
            }
        }
    }

    // Marca como invalido se nenhuma solucao valida foi encontrada
    if (!found) tabela[lin][col][0] = -1;
}



void reconstruct(int lin, int col, int v, const vector<vector<vector<int>>>& tabela, int n, string& result) {
    if (lin == col) { // Caso base de 1 elemento 
        result += to_string(v + 1);
        return;
    }

    if (v < 0 || v >= n || lin < 0 || col >= (int)tabela.size() || tabela[col][lin][v] == -1) {
        result = "Invalid"; // Marcar explicitamente como invalido
        return;
    }

    int k = tabela[col][lin][v];
    if (k == -1) {
        result = "Invalid"; // Marcar explicitamente como invalido
        return;
    }

    int left = tabela[col][lin][v + n];
    int right = tabela[col][lin][v + 2 * n];

    result += "("; 
    reconstruct(lin, k, left, tabela, n, result); // Parte esquerda
    if (result == "Invalid") return; // Parar se for invalido
    result += " ";
    reconstruct(k + 1, col, right, tabela, n, result); // Parte direita
    if (result == "Invalid") return; // Parar se for invalido
    result += ")"; 
}


int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 1; // Verificacao de entrada

    vector<vector<int>> matriz(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            scanf("%d", &matriz[i][j]);
        }
    }

    vector<int> sequence(m);
    for (int i = 0; i < m; ++i) {
        scanf("%d", &sequence[i]);
    }

    int target;
    if (scanf("%d", &target) != 1) return 1;

    // Caso especial: sequencia com apenas um valor
    if (m == 1) {
        if (sequence[0] == target) {
            printf("1\n%d\n", target);
        } else {
            printf("0\n");
        }
        return 0;
    }


    // Inicializa a tabela
    vector<vector<vector<int>>> tabela(m, vector<vector<int>>(m, vector<int>(n * 3, -1)));
    for (int i = 0; i < m; ++i) {
        tabela[i][i][0] = sequence[i] - 1; // Ajustar indice para 0-based
    }

    // Preenche a tabela para todos os subintervalos
    for (int len = 2; len <= m; ++len) {
        for (int lin = 0; lin <= m - len; ++lin) {
            int col = lin + len - 1;
            fill_square(lin, col, tabela, matriz, target - 1, n, 0, 1);
        }
    }

    // Reconstruir solucao
    string result;
    if (tabela[0][m - 1][target - 1] == -1) {
        printf("0\n");
    } else {
        reconstruct(0, m - 1, target - 1, tabela, n, result);
        if (result == "Invalid") {
            printf("0\n");
        } else {
            printf("1\n%s\n", result.c_str());
        }
    }

    return 0;
}