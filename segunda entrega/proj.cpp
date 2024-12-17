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
    set<pair<int, int>> visited; // Conjunto de estações e linhas visitadas (estação, linha)
    
    q.push(make_tuple(start, -1, -1)); // Começamos sem uma linha definida e sem mudanças (mudanças = -1, pois não conta a primeira estação)
    visited.insert(make_pair(start, -1)); // Marcamos a estação inicial sem linha como visitada

    while (!q.empty()) {
        tuple<int, int, int> current = q.front();
        q.pop();

        int station = get<0>(current);     // Estação atual
        int current_line = get<1>(current); // Linha atual
        int changes = get<2>(current);     // Número de mudanças até agora

        // Se chegarmos à estação de destino, retornamos o número de mudanças
        if (station == end) {
            return changes;
        }

        // Explorar todas as conexões da estação atual
        for (const Edge& edge : graph[station]) {
            // Se a estação e a linha ainda não foram visitadas
            if (visited.find(make_pair(edge.to, edge.line)) == visited.end()) {
                visited.insert(make_pair(edge.to, edge.line));
                if (current_line == edge.line) {
                    q.push(make_tuple(edge.to, current_line, changes)); // Continua na mesma linha
                } else {
                    q.push(make_tuple(edge.to, edge.line, changes + 1)); // Mudança de linha
                }
            }
        }
    }

    return -1; // Não há caminho entre start e end
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

    int result = metro_connectivity(graph);
    cout << result << endl;

    return 0;
}
