#include <cstdio>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const int MAX_N = 10001;

int n, m, l;
vector<pair<int, int>> graph[MAX_N]; // Lista de adjacência: (destino, linha)

// BFS para encontrar o menor número de mudanças de linha
int minLineChanges(int start, int end) {
    if (start == end) return 0;

    queue<pair<int, int>> q;          // {estação, linha atual}
    vector<int> changes(n + 1, -1);  // Mudanças para cada estação
    vector<int> visited(n + 1, 0);   // Visitados

    // Inicializar BFS com todas as linhas que partem da estação inicial
    for (const auto& edge : graph[start]) {
        int next_station = edge.first;
        int line = edge.second;
        q.push({next_station, line});
        changes[next_station] = 0; // Primeira linha não conta como mudança
        visited[next_station] = 1;
    }

    // BFS
    while (!q.empty()) {
        int station = q.front().first;
        int current_line = q.front().second;
        q.pop();

        // Verificar se chegamos ao destino
        if (station == end) {
            return changes[station];
        }

        // Expandir vizinhos
        for (const auto& edge : graph[station]) {
            int next_station = edge.first;
            int next_line = edge.second;
            int new_changes = changes[station] + (next_line != current_line);

            if (!visited[next_station] || new_changes < changes[next_station]) {
                visited[next_station] = 1;
                changes[next_station] = new_changes;
                q.push({next_station, next_line});
            }
        }
    }

    // Se não for possível chegar à estação de destino
    return -1;
}

int main() {
    // Ler entrada
    if (scanf("%d %d %d", &n, &m, &l) != 3 || n < 2 || m < 0 || l < 0) {
        return 0;
    }

    // Construir grafo
    vector<int> station_connections(n + 1, 0);
    for (int i = 0; i < m; i++) {
        int x, y, line;
        if (scanf("%d %d %d", &x, &y, &line) != 3) {
            return 0;
        }
        graph[x].push_back({y, line});
        graph[y].push_back({x, line});
        station_connections[x]++;
        station_connections[y]++;
    }

    // Verificar se há estações isoladas
    for (int i = 1; i <= n; i++) {
        if (station_connections[i] == 0) {
            printf("-1\n");
            return 0;
        }
    }

    // Calcular o número mínimo de mudanças de linha
    int result = minLineChanges(1, 2);
    printf("%d\n", result);

    return 0;
}
