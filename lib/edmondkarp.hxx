#pragma once

const int MAXN = 501; // Max vertices in max flow graph.

// Traverse p and augment flow with min edge found between v and s.
// Return min edge in path or 0 if no path could be found.
// Use recursive solution to avoid traversing path twice.
template<typename T>
T augment(int v, T min_edge, int s, vector<vector<T>> &res, const vi &p) {
    if (v == s) {
        return min_edge;
    }
    else if (p[v] != -1) {
        T f = augment(p[v], min(min_edge, res[p[v]][v]), s, res, p);
        res[p[v]][v] -= f; res[v][p[v]] += f;
        return f;
    }
    return T(0);
}

// Edmond Karps max flow algorithm.
// Return pair of maximum flow and residue matrix.
template<typename T>
pair<T, vector<vector<T>>> max_flow_res(
        const vector<vi> &adj_list, // Adjecency list (with bidirectional edges!) for faster lookup.
        const vector<vector<T>> &capacity, // Adjecency matrix with maximum capacity.
        int s, int t) // start -> end
{
    const T INF = numeric_limits<T>::max();  // > max edge value in flow graph.

    vector<vector<T>> res(capacity.begin(), capacity.end());
    vi p(adj_list.size());

    T mf(0);
    while (1) {
        // BFS find a valid spanning tree from s to t.
        bitset<MAXN> vis;
        vis[s] = true;
        queue<int> q; q.push(s);
        p.assign(MAXN, -1);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (u == t) break;
            for (int v : adj_list[u]) {
                if (!vis[v] && res[u][v] > T(0)) {
                    vis[v] = true;
                    q.push(v);
                    p[v] = u;
                }
            }
        }

        // Augment path backwards from t to s.
        T f = augment(t, INF, s, res, p);
        if (f == T(0)) break;
        mf += f;
    }

    return make_pair(mf, res);

}

// Return pair of maximum flow and flow as matrix.
template<typename T>
pair<T, vector<vector<T>>> max_flow(
        const vector<vi> &adj_list, // Adjecency list (with bidirectional edges!) for faster lookup.
        const vector<vector<T>> &capacity, // Adjecency matrix with maximum capacity.
        int s, int t) // start -> end
{
    auto f = max_flow_res(adj_list, capacity, s, t);
    T mf = f.first;
    auto res = f.second;

    // Construct flow matrix.
    const int n = adj_list.size();
    vector<vector<T>> flow(n, vector<T>(n, T(0)));
    for (int u = 0; u < n; ++u) {
        for (int v : adj_list[u]) {
            T f = capacity[u][v] - res[u][v];
            if (f > T(0))
                flow[u][v] = f;
        }
    }

    return make_pair(mf, flow);
}

