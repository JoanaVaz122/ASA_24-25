#include <cstdio>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

int n, m;

void iterativeDFS(vector<vector<int>>& graph, stack<int>& exitOrder, vector<int>& visited) {
    stack<int> dfsStack;

    for (int i = 1; i <= n; ++i) {
        if (visited[i] == 0) {
            dfsStack.push(i);

            while (!dfsStack.empty()) {
                int current = dfsStack.top();
                dfsStack.pop();

                if (visited[current] == 0) {
                    visited[current] = 1;
                    dfsStack.push(current);

                    for (int neighbor : graph[current]) {
                        if (visited[neighbor] == 0) {
                            dfsStack.push(neighbor);
                        }
                    }
                } else if (visited[current] == 1) {
                    visited[current] = 2;
                    exitOrder.push(current);
                }
            }
        }
    }
}

void iterativeDFS2(vector<vector<int>>& graphT, stack<int>& exitOrder, vector<int>& maxJumps, vector<int>& visited) {
    while (!exitOrder.empty()) {
        int startNode = exitOrder.top();
        exitOrder.pop();

        if (visited[startNode] == 0) {
            stack<int> dfsStack;
            dfsStack.push(startNode);
            int localMax = 0;

            while (!dfsStack.empty()) {
                int current = dfsStack.top();
                dfsStack.pop();

                if (visited[current] == 0) {
                    visited[current] = 1;

                    for (int neighbor : graphT[current]) {
                        if (visited[neighbor] == 0) {
                            dfsStack.push(neighbor);
                        } else {
                            // Atualizar os "saltos máximos" considerando os vizinhos já visitados
                            maxJumps[current] = max(maxJumps[current], maxJumps[neighbor] + 1);
                        }
                    }

                    localMax = max(localMax, maxJumps[current]);
                }
            }

            // Atualizar o salto máximo global
            maxJumps[startNode] = localMax;
        }
    }
}

int main() {
    if (scanf("%d %d", &n, &m) != 2 || n < 1 || m < 0) {
        return 0;
    }

    vector<vector<int>> graph(n + 1);
    vector<vector<int>> graphT(n + 1);
    stack<int> exitOrder;
    vector<int> maxJumps(n + 1, 0);
    vector<int> visited(n + 1, 0);

    for (int i = 0; i < m; ++i) {
        int x, y;
        if (scanf("%d %d", &x, &y) != 2) {
            return 0;
        }

        graph[x].push_back(y);
        graphT[y].push_back(x);
    }

    // Primeira DFS para determinar a ordem de saída
    iterativeDFS(graph, exitOrder, visited);

    // Resetar o vetor de visitados
    fill(visited.begin(), visited.end(), 0);

    // Segunda DFS para calcular os saltos máximos
    iterativeDFS2(graphT, exitOrder, maxJumps, visited);

    // Obter o maior número de saltos
    int maxJump = *max_element(maxJumps.begin(), maxJumps.end());
    printf("%d\n", maxJump);

    return 0;
}
