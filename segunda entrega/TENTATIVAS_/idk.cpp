#include <cstdio>
#include <vector>
#include <queue>
#include <climits>
#include <unordered_map>
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
        if (min_changes == INT_MAX) {
            printf("-1\n");
            return 0;
        }

        max_changes = max(max_changes, min_changes);
    }

    return max_changes;
}

int main() {
    int n, m, l;

    if (scanf("%d %d %d", &n, &m, &l) != 3 || n < 2 || m < 0 || l < 0) {
        printf("-1\n");
        return 0;
    }

    // Grafo onde cada conexão inclui a linha associada
    vector<vector<pair<int, int>>> graph(n);

    // Mapeamento das conexões entre linhas para cada estação
    unordered_map<int, vector<pair<int, int>>> line_connections;

    // Ler as conexões
    for (int i = 0; i < m; ++i) {
        int x, y, line;
        if (scanf("%d %d %d", &x, &y, &line) != 3 || x < 1 || y < 1 || x > n || y > n || line < 1 || line > l) {
            printf("-1\n");
            return 0;
        }
        --x; --y; // Convertido para índice baseado em 0
        graph[x].push_back({y, line});
        graph[y].push_back({x, line});

        // Adicionar conexões de linha para permitir trocas
        line_connections[x].push_back({y, line});
        line_connections[y].push_back({x, line});
    }

    // Adicionar trocas de linha (conexões virtuais entre diferentes linhas na mesma estação)
    for (int station = 0; station < n; ++station) {
        auto& connections = line_connections[station];
        for (size_t i = 0; i < connections.size(); ++i) {
            for (size_t j = i + 1; j < connections.size(); ++j) {
                int line1 = connections[i].second;
                int line2 = connections[j].second;
                if (line1 != line2) {
                    graph[station].push_back({station, line2});
                    graph[station].push_back({station, line1});
                }
            }
        }
    }

    // Caso haja somente uma estação
    if (n == 1) {
        printf("0\n");
        return 0;
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

    // Se alguma estação não estiver conectada, retornar -1
    if (count != n || m == 0) {
        printf("-1\n");
        return 0;
    }

    // Verificar se todas as estações estão na mesma linha
    bool same_line = true;
    for (int i = 0; i < n; ++i) {
        int line = -1;
        for (auto conn : graph[i]) {
            if (line == -1) {
                line = conn.second;
            } else if (line != conn.second) {
                same_line = false;
                break;
            }
        }
        if (!same_line) break;
    }

    if (same_line) {
        printf("0\n");
        return 0;
    }

    // Calcular o número máximo de trocas entre todas as estações
    int max_changes = bfs(graph, n, l);

    printf("%d\n", max_changes);
    return 0;
}
