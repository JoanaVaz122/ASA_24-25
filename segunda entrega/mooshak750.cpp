#include <cstdio>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

// BFS to check if the graph is connected
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

vector<vector<int>> memo; // Memoization table

// BFS to calculate minimum changes between stations
int bfs(const vector<vector<pair<int, int>>>& graph, int start, int n, int l) {
    if (memo[start][0] != -1) return memo[start][0]; // Check if result is already computed

    queue<pair<int, int>> q;  // Queue: {station, current line}
    vector<vector<int>> dist(n, vector<int>(l + 1, INT_MAX)); // Distance table

    // Initialize BFS from the starting station
    for (auto conn : graph[start]) {
        int next_station = conn.first;
        int next_line = conn.second;
        q.push({next_station, next_line});
        dist[next_station][next_line] = 0; // No cost to move to the initial neighbors
    }

    while (!q.empty()) {
        int station = q.front().first;
        int line = q.front().second;
        q.pop();

        for (auto conn : graph[station]) {
            int next_station = conn.first;
            int next_line = conn.second;

            // Calculate the cost to move to the next station and line
            int cost = dist[station][line] + (line != next_line);

            if (cost < dist[next_station][next_line]) {
                dist[next_station][next_line] = cost;
                q.push({next_station, next_line});
            }
        }
    }

    // Find the minimum cost to reach all stations
    int max_changes = INT_MIN;
    for (int station = 0; station < n; ++station) {
        int min_changes = INT_MAX;
        for (int line = 1; line <= l; ++line) {
            min_changes = min(min_changes, dist[station][line]);
        }
        max_changes = max(max_changes, min_changes);
    }

    memo[start][0] = max_changes; // Store the result in memoization table
    return max_changes;
}

int main() {
    int n, m, l;
    scanf("%d %d %d", &n, &m, &l);

    vector<vector<pair<int, int>>> graph(n);
    memo.assign(n, vector<int>(1, -1)); // Initialize memoization table

    // Read connections
    for (int i = 0; i < m; ++i) {
        int x, y, line;
        scanf("%d %d %d", &x, &y, &line);
        --x; --y; // Convert to 0-based index
        graph[x].push_back({y, line});
        graph[y].push_back({x, line});
    }

    // Check if the graph is connected
    if (!is_graph_connected(graph, n)) {
        printf("-1\n");
        return 0;
    }

    // Calculate maximum connectivity between all stations
    int max_changes = 0;
    for (int i = 0; i < n; ++i) {
        max_changes = max(max_changes, bfs(graph, i, n, l));
    }

    printf("%d\n", max_changes);
    return 0;
}
