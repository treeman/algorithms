#pragma once

// Impossible values will be of distance INF.

// Return a vector of pair<dist, parent>.
// Take in an adjecency list of type pair<node, weight>.
// Variant may leave duplicate instances of a node in frontier.
template<typename T>
vector<pair<T, int>> shortest_path(vector<vector<pair<int, T>>> adj_list, int start) {
    typedef pair<T, int> State;
    vector<State> path(adj_list.size(), make_pair(INF, -1));
    path[start] = State(T(0), 0);

    // Queue of <dist, node>.
    priority_queue<State, vector<State>, greater<State> > pq;
    pq.push(State(T(0), start));
    while (!pq.empty()) {
        T dist = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (dist > path[u].first) continue;

        for (auto edge : adj_list[u]) {
            int v = edge.first;
            T next_cost = dist + edge.second;
            if (next_cost < path[v].first) {
                path[v] = State(next_cost, u);
                pq.push(State(next_cost, v));
            }
        }
    }
    return path;
}


