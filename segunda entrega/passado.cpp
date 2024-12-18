#include <cstdio>
#include <vector>
#include <queue>
#include <climits>
#include <set>
#include <algorithm>

using namespace std;

// BFS para verificar conectividade de todo o grafo
bool is_graph_connected(const vector<vector<pair<int, int>>>& graph, int n) {
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

    return count == n;
}

// BFS para calcular mudanças mínimas entre estações
int bfs(const vector<vector<pair<int, int>>>& graph, int start, int n, int l) {
    queue<pair<int, int>> q;  // Fila: {estação, linha atual}
    vector<vector<int>> dist(n, vector<int>(l + 1, INT_MAX));

    // Inicializa BFS a partir da estação inicial
    for (auto conn : graph[start]) {
        int next_station = conn.first;
        int next_line = conn.second;
        q.push({next_station, next_line});
        dist[next_station][next_line] = 0;
    }

    while (!q.empty()) {
        int station = q.front().first;
        int line = q.front().second;
        q.pop();

        for (auto conn : graph[station]) {
            int next_station = conn.first;
            int next_line = conn.second;
            int cost = dist[station][line] + (line != next_line);

            if (cost < dist[next_station][next_line]) {
                dist[next_station][next_line] = cost;
                q.push({next_station, next_line});
            }
        }
    }

    // Retorna a maior distância encontrada para esta estação
    int max_changes = 0;
    for (int line = 1; line <= l; ++line) {
        max_changes = max(max_changes, *min_element(dist[line].begin(), dist[line].end()));
    }
    return max_changes;
}

int main() {
    int n, m, l;
    scanf("%d %d %d", &n, &m, &l);

    vector<vector<pair<int, int>>> graph(n);

    // Leitura das conexões
    for (int i = 0; i < m; ++i) {
        int x, y, line;
        scanf("%d %d %d", &x, &y, &line);
        --x; --y; // Ajusta índices para base 0
        graph[x].push_back({y, line});
        graph[y].push_back({x, line});
    }

    // Verifica se o grafo é conectado
    if (!is_graph_connected(graph, n)) {
        printf("-1\n");
        return 0;
    }

    // Caso particular: verifica se todas as estações estão em uma única linha
    if (m == n - 1) {
        printf("0\n");
        return 0;
    }

    // Calcula conectividade máxima entre todas as estações
    int max_changes = 0;
    for (int i = 0; i < n; ++i) {
        max_changes = max(max_changes, bfs(graph, i, n, l));
    }

    printf("%d\n", max_changes);
    return 0;
}
