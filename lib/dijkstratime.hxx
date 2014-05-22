#pragma once

// Impossible values will be of distance INF.

// Return a vector of pair<dist, parent>.
// Take in an adjecency list of type pair<node, starttime, period, traverse-time>.
// Edges will be walkable at starttime + t * period for all integers t.
// Variant may leave duplicate instances of a node in frontier.
template<typename T>
vector<pair<T, int>> shortest_path(
        const vector<vector<tuple<int, int, int, T>>> &adj_list,
        int start)
{
    typedef pair<T, int> Node;

    vector<Node> path(adj_list.size(), make_pair(INF, -1));
    path[start] = Node(T(0), 0); // time to reach/parent

    // Queue of <time, node>.
    priority_queue<Node, vector<Node>, greater<Node> > pq;
    pq.push(Node(T(0), start));
    while (!pq.empty()) {
        T t = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (t > path[u].first) continue;

        for (auto edge : adj_list[u]) {
            int v, t0, p, d;
            tie(v, t0, p, d) = edge;

            T next_start;
            if (t0 >= t || p == 0) {
                next_start = t0;
            }
            else { // Find next period, t0 < t
                const T diff = t - t0;
                next_start = t0 + (diff / p) * p + (diff % p ? p : 0);
            }
            const T next_time = next_start + d;

            if (next_time >= t && next_time < path[v].first) {
                path[v] = Node(next_time, u);
                pq.push(Node(next_time, v));
            }
        }
    }
    return path;
}


