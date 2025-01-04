#include <iostream>
#include <vector>
#include <set>
#include <limits.h>

using namespace std;

void floydWarshall(vector<vector<int>>& dist, int L) {
    for (int k = 1; k <= L; ++k) {
        for (int i = 1; i <= L; ++i) {
            for (int j = 1; j <= L; ++j) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
}

int main() {
    int V, E, L;
    if(scanf("%d %d %d", &V, &E, &L)!= 3 || V < 2 || E < 0 || L<0){
        printf("-1\n");
        return 0;
    }

    if (V == 1) { // Case where there is only one station
        printf("0\n");
        return 0;
    }

    vector<set<int>> stationLines(V + 1);
    vector<set<int>> lineIntersections(L + 1);

    for (int i = 0; i < E; ++i) {
        int V1, V2, LN;
        if (scanf("%d %d %d", &V1, &V2, &LN) != 3 || V1 < 1 || V2 < 1 || V1 > V || V2 > V || LN < 1 || LN > L) {
            printf("-1\n");
            return 0;
        }
        stationLines[V1].insert(LN);
        stationLines[V2].insert(LN);
    }

    // Optimized calculation of line intersections
    for (int i = 1; i <= V; ++i) {
        vector<int> lines(stationLines[i].begin(), stationLines[i].end());
        for (size_t j = 0; j < lines.size(); ++j) {
            for (size_t k = j + 1; k < lines.size(); ++k) {
                lineIntersections[lines[j]].insert(lines[k]);
                lineIntersections[lines[k]].insert(lines[j]);
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
            printf("0\n");
            return 0;
        }
    }

    // Check if there is any station not crossed by any lines
    for (int st = 1; st <= V; ++st) {
        if (stationLines[st].empty()) {
            printf("-1\n");
            return 0;
        }
    }

    // Check if there are two lines that never intersect
    for (int i = 1; i <= L; ++i) {
        for (int j = i + 1; j <= L; ++j) {
            if (lineIntersectionsVec[i].empty() || lineIntersectionsVec[j].empty()) {
                printf("-1\n");
                return 0;
            }
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
            }else{
                return -1; // Unreachable station
            }
        }
    }

    printf("%d\n", maxDistance);

    return 0;
}
