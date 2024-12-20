#include <iostream>
#include <vector>
#include <set>
#include <limits.h>

using namespace std;

void floydWarshall(vector<vector<int>>& dist, int L) {
    for (int k = 1; k <= L; ++k) {
        for (int i = 1; i <= L; ++i) {
            for (int j = 1; j <= L; ++j) { // Corrected loop condition
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
}

int main() {
    int V, E, L;
    cin >> V >> E >> L;

    vector<set<int>> stationLines(V + 1); // Vector of sets to store line numbers for each station
    vector<set<int>> lineIntersections(L + 1); // Vector of sets to store intersecting lines for each line

    for (int i = 0; i < E; ++i) {
        int V1, V2, LN;
        cin >> V1 >> V2 >> LN;
        stationLines[V1].insert(LN);
        stationLines[V2].insert(LN);
    }

    // Calculate line intersections
    for (int i = 1; i <= V; ++i) {
        for (auto line1 : stationLines[i]) {
            for (auto line2 : stationLines[i]) {
                if (line1 != line2) {
                    lineIntersections[line1].insert(line2);
                }
            }
        }
    }

    // Convert sets to vectors for the final result
    vector<vector<int>> stationLinesVec(V + 1);
    vector<vector<int>> lineIntersectionsVec(L + 1);

    for (int i = 1; i <= V; ++i) {
        stationLinesVec[i] = vector<int>(stationLines[i].begin(), stationLines[i].end());
    }

    for (int i = 1; i <= L; ++i) {
        lineIntersectionsVec[i] = vector<int>(lineIntersections[i].begin(), lineIntersections[i].end());
    }

    // Check if there is any line that goes through every station
    for (int ln = 1; ln <= L; ++ln) {
        bool allStationsCovered = true;
        for (int st = 1; st <= V; ++st) {
            if (stationLines[st].find(ln) == stationLines[st].end()) {
                allStationsCovered = false;
                break;
            }
        }
        if (allStationsCovered) {
            cout << 0 << endl;
            return 0;
        }
    }

    // Check if there is any station not crossed by any lines
    for (int st = 1; st <= V; ++st) {
        if (stationLines[st].empty()) {
            cout << -1 << endl;
            return 0;
        }
    }

    // Initialize distance matrix for Floyd-Warshall algorithm
    vector<vector<int>> dist(L + 1, vector<int>(L + 1, INT_MAX));
    for (int i = 1; i <= L; ++i) {
        dist[i][i] = 0;
        for (int j : lineIntersectionsVec[i]) {
            dist[i][j] = 1;
        }
    }

    // Run Floyd-Warshall algorithm to find shortest paths between all pairs of lines
    floydWarshall(dist, L);

    // Find the largest value in the distance matrix that is not INT_MAX
    int maxDistance = 0;

    for (int i = 1; i <= V; ++i) {
        for (int j = i + 1; j <= V; ++j) {
            int minDistance = INT_MAX;
            for (int line1 : stationLinesVec[i]) {
                for (int line2 : stationLinesVec[j]) {
                    if (dist[line1][line2] != INT_MAX) {
                        minDistance = min(minDistance, dist[line1][line2]);
                    }
                }
            }
            if (minDistance != INT_MAX) {
                maxDistance = max(maxDistance, minDistance);
            }
        }
    }

    cout << maxDistance << endl;

    return 0;
}
