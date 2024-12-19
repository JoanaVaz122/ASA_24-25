#include <cstdio>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

// Função para calcular o número mínimo de trocas de linha entre as estações
int bfs(const vector<vector<pair<int, int>>>& graph, int n, int l) {
    vector<vector<int>> dist(n, vector<int>(l + 1, INT_MAX));
    queue<pair<int, int>> q; // Fila: {estação, linha atual}

    // Inicializar BFS a partir da estação inicial (0)
    for (auto conn : graph[0]) {
        int next_station = conn.first;
        int next_line = conn.second;
        dist[next_station][next_line] = 0; // Sem trocas inicialmente
        q.push({next_station, next_line});
    }

    while (!q.empty()) {
        int station = q.front().first;
        int line = q.front().second;
        q.pop();

        for (auto conn : graph[station]) {
            int next_station = conn.first;
            int next_line = conn.second;

            // Somente considere trocas de linha
            int cost = dist[station][line] + (line != next_line);

            if (cost < dist[next_station][next_line]) {
                dist[next_station][next_line] = cost;
                q.push({next_station, next_line});
            }
        }
    }

    // Encontrar o custo máximo entre todas as estações
    int max_changes = 0;
    for (int station = 0; station < n; ++station) {
        int min_changes = INT_MAX;
        for (int line = 1; line <= l; ++line) {
            min_changes = min(min_changes, dist[station][line]);
        }
        max_changes = max(max_changes, min_changes);
    }

    return max_changes;
}

int main() {
    int n, m, l;
    scanf("%d %d %d", &n, &m, &l);

    vector<vector<pair<int, int>>> graph(n);

    // Ler as conexões
    for (int i = 0; i < m; ++i) {
        int x, y, line;
        scanf("%d %d %d", &x, &y, &line);
        --x; --y; // Convertido para índice baseado em 0
        graph[x].push_back({y, line});
        graph[y].push_back({x, line});
    }

    // Verificar conectividade do grafo
    vector<int> visited(n, 0);
    queue<int> q;
    q.push(0);
    visited[0] = 1;
    int count = 1;

    while (!q.empty()) {
        int station = q.front();
        q.pop();

        for (auto conn : graph[station]) {
            int next_station = conn.first;
            if (!visited[next_station]) {
                visited[next_station] = 1;
                q.push(next_station);
                count++;
            }
        }
    }

    if (count != n) {
        printf("-1\n");
        return 0;
    }

    // Calcular o número máximo de trocas entre todas as estações
    int max_changes = bfs(graph, n, l);

    printf("%d\n", max_changes);
    return 0;
}
