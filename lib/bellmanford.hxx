#pragma once

const int INF = numeric_limits<int>::max();
const int NEG_INF = numeric_limits<int>::min();

// Bellman-Ford's algorithm for single source shortest path.
// Return a vector of pair<dist, parent>.
// Take in an adjecency list of type pair<node, weight>.
template<typename T>
vector<pair<T, int>> shortest_path(vector<vector<pair<int, T>>> adj_list, int start) {
    typedef pair<T, int> Node;

    vector<Node> path(adj_list.size(), make_pair(INF, -1));
    path[start] = Node(0, 0);

    // Relax n - 1 times.
    for (int i = 0; i < (int)adj_list.size() - 1; ++i) {
        for (int u = 0; u < (int)adj_list.size(); ++u) {
            for (auto edge : adj_list[u]) {
                int v = edge.first;
                T next_cost = edge.second + path[u].first;
                if (path[u].first != INF && next_cost < path[v].first) {
                    path[v] = Node(next_cost, u);
                }
            }
        }
    }

    // If still possible to relax the nth, a negative cycle is found.
    for (int u = 0; u < (int)adj_list.size(); ++u) {
        for (auto edge : adj_list[u]) {
            int v = edge.first;
            T cost = edge.second;
            // Negative cycle found.
            if (path[u].first != INF && path[v].first > path[u].first + cost) {
                // Do bfs and mark everything reachable as -Infinity.
                queue<int> q;
                q.push(u);
                while (!q.empty()) {
                    int x = q.front(); q.pop();
                    path[x].first = NEG_INF;
                    for(auto edge: adj_list[x]) {
                        if (path[edge.first].first != NEG_INF)
                            q.push(edge.first);
                    }
                }
            }
        }
    }

    return path;
}

