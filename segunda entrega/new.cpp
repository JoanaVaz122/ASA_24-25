#include <cstdio>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

// Função para calcular o número mínimo de trocas de linhas entre as estações
int bfs(const vector<vector<pair<int, int>>>& graph, int start, int n, int l) {
    queue<pair<int, int>> q;  // Fila: {estação, linha atual}
    vector<vector<int>> dist(n, vector<int>(l + 1, INT_MAX)); // Tabela de distâncias

    // Inicializar BFS a partir da estação inicial
    for (auto conn : graph[start]) {
        int next_station = conn.first;
        int next_line = conn.second;
        q.push({next_station, next_line});
        dist[next_station][next_line] = 0; // Sem custo para mover para os vizinhos iniciais
    }

    while (!q.empty()) {
        int station = q.front().first;
        int line = q.front().second;
        q.pop();

        for (auto conn : graph[station]) {
            int next_station = conn.first;
            int next_line = conn.second;

            // Calcular o custo para mover para a próxima estação e linha
            int cost = dist[station][line] + (line != next_line);

            if (cost < dist[next_station][next_line]) {
                dist[next_station][next_line] = cost;
                q.push({next_station, next_line});
            }
        }
    }

    // Encontrar o custo mínimo para alcançar todas as estações
    int max_changes = INT_MIN;
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
    vector<int> degrees(n, 0);  // Para verificar se alguma estação não tem ligações

    // Ler as conexões
    for (int i = 0; i < m; ++i) {
        int x, y, line;
        scanf("%d %d %d", &x, &y, &line);
        --x; --y; // Convertido para índice baseado em 0
        graph[x].push_back({y, line});
        graph[y].push_back({x, line});
        degrees[x]++;
        degrees[y]++;
    }

    // Verificar se todas as estações estão conectadas (agora dentro da função main)
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
                if (count == n) {
                    break; // Se já visitou todas as estações, pode parar a busca
                }
            }
        }
    }

    // Se o número de estações visitadas não for igual a n, o grafo não está conectado
    if (count != n) {
        printf("-1\n");
        return 0;
    }

    // Verificar se alguma estação não tem ligações
    for (int i = 0; i < n; ++i) {
        if (degrees[i] == 0) {
            printf("-1\n");
            return 0;
        }
    }

    // Se houver apenas uma estação, não há necessidade de mudanças de linha
    if (n == 1) {
        printf("0\n");
        return 0;
    }

    // Verificar se todas as conexões são feitas pela mesma linha
    bool all_same_line = true;
    int first_line = -1;
    for (int i = 0; i < n; ++i) {
        for (auto conn : graph[i]) {
            if (first_line == -1) { 
                first_line = conn.second;
            } else if (conn.second != first_line) {
                all_same_line = false;
                break;
            }
        }
        if (!all_same_line) break;
    }

    if (all_same_line) {
        printf("0\n");
        return 0;
    }

    // Calcular o número máximo de trocas entre todas as estações
    int max_changes = 0;
    for (int i = 0; i < n; ++i) {
        max_changes = max(max_changes, bfs(graph, i, n, l));
    }

    printf("%d\n", max_changes);
    return 0;
}
