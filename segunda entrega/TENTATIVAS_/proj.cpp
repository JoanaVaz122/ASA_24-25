#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <tuple>

using namespace std;

struct Edge {
    int to, line; // Estação de destino e linha
};

int bfs(const vector<vector<Edge>> &graph, int start, int end, int n) {
    queue<tuple<int, int, int>> q; // {estação atual, linha atual, contador de mudanças}
    set<int> visited; // Conjunto de estações visitadas

    q.push(make_tuple(start, -1, -1)); // Começamos sem uma linha definida e sem mudanças (mudanças = -1, pois não conta a primeira estação)
    visited.insert(start); // Marcamos a estação inicial como visitada

    while (!q.empty()) {
        tuple<int, int, int> current = q.front();
        q.pop();

        int station = get<0>(current);     // Estação atual
        int current_line = get<1>(current); // Linha atual
        int changes = get<2>(current);     // Número de mudanças até agora

        // Se chegarmos à estação de destino, adicionamos o número de mudanças ao vetor
        if (station == end) {
            return changes;
        }

        // Explorar todas as conexões da estação atual
        for (const Edge& edge : graph[station]) {
            // Se a estação ainda não foi visitada
            if (visited.find(edge.to) == visited.end()) {
                visited.insert(edge.to);
                if (current_line == edge.line) {
                    q.push(make_tuple(edge.to, current_line, changes)); // Continua na mesma linha
                } else {
                    q.push(make_tuple(edge.to, edge.line, changes + 1)); // Mudança de linha
                }
            }
        }
    }

    return -1;
}

int metro_connectivity(const vector<vector<Edge>> &graph) {
    int n = graph.size();
    int max_changes = 0;

    // Testar todos os pares de estações
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                int changes = bfs(graph, i, j, n);
                if (changes == -1) return -1; // Se duas estações não estão conectadas
                max_changes = max(max_changes, changes);
            }
        }
    }

    return max_changes;
}

bool is_line_connecting_all_stations(const vector<vector<Edge>> &graph, int n) {
    for (int line = 0; line < n; ++line) {
        vector<bool> visited(n, false);
        queue<int> q;
        int start_station = -1;

        // Find a starting station for the current line
        for (int i = 0; i < n; ++i) {
            for (const Edge& edge : graph[i]) {
                if (edge.line == line) {
                    start_station = i;
                    break;
                }
            }
            if (start_station != -1) break;
        }

        if (start_station == -1) continue; // No station found for this line

        q.push(start_station);
        visited[start_station] = true;
        int count = 1;

        while (!q.empty()) {
            int station = q.front();
            q.pop();

            for (const Edge& edge : graph[station]) {
                if (edge.line == line && !visited[edge.to]) {
                    visited[edge.to] = true;
                    q.push(edge.to);
                    count++;
                }
            }
        }

        if (count == n) return true; // All stations are connected by this line
    }

    return false; // No single line connects all stations
}

int main() {
    int n, m, l;
    cin >> n >> m >> l;

    vector<vector<Edge>> graph(n);

    // Leitura das conexões
    for (int i = 0; i < m; ++i) {
        int x, y, line;
        cin >> x >> y >> line;
        --x; --y; // Ajuste para índices baseados em 0
        graph[x].push_back((Edge){y, line});
        graph[y].push_back((Edge){x, line});
    }

    // Verificar se existe uma linha que conecta todas as estações
    if (is_line_connecting_all_stations(graph, n)) {
        cout << 0 << endl;
        return 0;
    }

    int result = metro_connectivity(graph);
    cout << result << endl;

    return 0;
}
