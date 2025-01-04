#include <cstdio>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

int n, m, l;

int bfs(const vector<vector<pair<int, int>>>& graph, int start, int end) {
    // dist[estacao][linha] = número de mudanças
    vector<vector<int>> dist(n + 1, vector<int>(l + 1, INT_MAX)); 
    queue<pair<int, int>> q;  // Par: (estação, linha)
    
    // Inicializa a BFS para todas as linhas que saem da estação 1
    for (auto& p : graph[start]) {
        int neighbor = p.first;
        int line = p.second;
        dist[start][line] = 0;  // Nenhuma mudança na estação inicial
        q.push({neighbor, line});
    }

    while (!q.empty()) {
        int station = q.front().first;
        int line = q.front().second;
        q.pop();

        // Se já encontramos a estação de destino, retornamos o resultado
        if (station == end) {
            return dist[station][line];
        }

        // Explora os vizinhos da estação atual
        for (auto& p : graph[station]) {
            int neighbor = p.first;
            int newLine = p.second;
            int newDist = dist[station][line] + (newLine != line);  // Se mudar de linha, conta uma mudança

            // Só adiciona à fila se a nova distância for menor
            if (newDist < dist[neighbor][newLine]) {
                dist[neighbor][newLine] = newDist;
                q.push({neighbor, newLine});
            }
        }
    }

    return -1;  // Se não encontrar uma solução
}

int main() {
    if (scanf("%d %d %d", &n, &m, &l) != 3 || n < 2 || m < 0 || l < 0) {
        return 0;
    }

    vector<vector<pair<int, int>>> graph(n + 1);

    // Preencher o grafo com as ligações
    for (int i = 0; i < m; ++i) {
        int x, y, line;
        if (scanf("%d %d %d", &x, &y, &line) != 3) {
            return 0;
        }
        graph[x].push_back({y, line});
        graph[y].push_back({x, line});  // Grafo não direcionado
    }

    // Iniciar a busca pelo índice de conectividade
    int result = bfs(graph, 1, n);

    // Verificar os casos especiais
    if (result == -1) {
        printf("-1\n");
    } else {
        printf("%d\n", result);
    }

    return 0;
}
