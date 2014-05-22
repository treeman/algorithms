#pragma once

#include "edmondkarp.hxx"

template<typename T>
set<int> min_cut(
        const vector<vi> &adj_list, // Adjecency list (with bidirectional edges) for faster lookup.
        const vector<vector<T>> &capacity, // Adjecency matrix with maximum capacity.
        int s, int t) // start -> end
{
    vector<vector<T>> res = max_flow_res(adj_list, capacity, s, t).second;

    vi seen(adj_list.size(), 0);
    set<int> cut;
    queue<int> q;
    q.push(s);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        cut.insert(u);
        seen[u] = 1;

        for (int v : adj_list[u]) {
            int r = res[u][v];
            if (!seen[v] && r > 0) {
                q.push(v);
                seen[v] = 1;
            }
        }
    }

    return cut;
}

