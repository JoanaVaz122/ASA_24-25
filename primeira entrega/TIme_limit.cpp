#include <vector>
#include <string>
#include <tuple>
#include <chrono> 

using namespace std;

// Calcular o resultado da operação binária
int operate(const vector<vector<int>>& table, int a, int b) {
    return table[a - 1][b - 1];
}

// Encontrar a parentização válida
bool solve(const vector<int>& sequence, const vector<vector<int>>& table, int result,
           vector<vector<vector<bool>>>& dp, vector<vector<vector<string>>>& parens, int start, int end) {
    //printf("Verificando subarray [%d, %d] para resultado %d\n", start, end, result);
    
    if (dp[start][end][result]) {
      //  printf("Subarray [%d, %d] com resultado %d já resolvido. Valor armazenado: %s\n",
              // start, end, result, parens[start][end][result].c_str());
        return true;
    }

    if (start == end) {
        if (sequence[start] == result) {
            parens[start][end][result] = to_string(sequence[start]);
            dp[start][end][result] = true;
          //  printf("Base: sequência[%d] = %d corresponde ao resultado %d\n", start, sequence[start], result);
            return true;
        }
        //printf("Base: sequência[%d] = %d NÃO corresponde ao resultado %d\n", start, sequence[start], result);
        return false;
    }

    for (int mid = end - 1; mid >= start; --mid) {
        //printf("Dividindo em [%d, %d] e [%d, %d]\n", start, mid, mid + 1, end);
        for (size_t leftResult = 1; leftResult <= table.size(); ++leftResult) {
            for (size_t rightResult = 1; rightResult <= table.size(); ++rightResult) {
                if (solve(sequence, table, leftResult, dp, parens, start, mid) &&
                    solve(sequence, table, rightResult, dp, parens, mid + 1, end) &&
                    operate(table, leftResult, rightResult) == result) {

                    dp[start][end][result] = true;

                    parens[start][end][result] = "(" + parens[start][mid][leftResult] + " " +
                                                 parens[mid + 1][end][rightResult] + ")";
          //          printf("Parênteses colocados para [%d, %d] com resultado %d: %s\n",
                          // start, end, result, parens[start][end][result].c_str());
                    return true;
                }
            }
        }
    }

   // printf("Subarray [%d, %d] não conseguiu produzir o resultado %d\n", start, end, result);
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

 // Medir o tempo de execução
 //   auto start_time = chrono::high_resolution_clock::now();

   // if (solve(sequence, table, target, dp, parens, 0, m - 1)) {
        //auto end_time = chrono::high_resolution_clock::now();
        //chrono::duration<double> elapsed = end_time - start_time;

     //   printf("1\n%s\n", parens[0][m - 1][target].c_str());
       // printf("Tempo de execução: %.6f segundos\n", elapsed.count());
    //} else {
        //auto end_time = chrono::high_resolution_clock::now();
      //  chrono::duration<double> elapsed = end_time - start_time;

      //  printf("0\n");
     //   printf("Tempo de execução: %.6f segundos\n", elapsed.count());
    //}



    return 0;
}
