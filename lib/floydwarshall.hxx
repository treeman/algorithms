#pragma once

// Floyd Warshall's all pairs shortest path.
// Will handle negative weights and signal it with NEG_INF.
// Impossible will be INF. Directed edges.
// Input and output as adjecency matrix.

template<typename T>
vector<vector<T> > shortest_path_all_pairs(const vector<vector<T> > &adj_matrix) {
    const T INF = numeric_limits<T>::max();
    const T NEG_INF = numeric_limits<T>::min();
    const int n = adj_matrix.size();
    vector<vector<T> > dist(adj_matrix.begin(), adj_matrix.end());

    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (dist[i][k] != INF && dist[k][j] != INF &&
                        dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    for (int k = 0; k < n; ++k) {
        if (dist[k][k] < 0) {
            dist[k][k] = NEG_INF;
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (dist[i][k] != INF && dist[k][j] != INF)
                        dist[i][j] = NEG_INF;
                }
            }
        }
    }

    return dist;
}


