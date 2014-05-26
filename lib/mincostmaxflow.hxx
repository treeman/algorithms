#pragma once

// Calculates the max flow through the graph.
// Take in an adjecency list (backward edges not necessary),
// capacity and cost as matrixes.
// Returns <mincost, maxflow, actual flow>.
// Flow will be represented as an adjecency matrix.
// Uses relabelling with dijkstra. Costs must be nonnegative!
template<typename Cost, typename Flow>
tuple<Cost, Flow, vector<vector<Flow>>> max_flow_matrix(
        const vector<vi> &adj_list, // Adjecency list.
        const vector<vector<Flow>> &capacity, // Adjecency matrix with capacity.
        const vector<vector<Cost>> &cost, // Adjecency matrix with cost.
        int s, int t) // start -> end
{
    const int n = capacity.size();
    const Flow INF = numeric_limits<Flow>::max();  // > max edge value in flow graph.

    // We need to be able to enumerate backward edges as well.
    vector<vi> back_adj_list(n, vi());
    for (int u = 0; u < n; ++u) {
        for (int v : adj_list[u])
            back_adj_list[v].push_back(u);
    }

    vector<vector<Flow>> flow(n, vector<Flow>(n, 0)); // Current flow.
    vector<Cost> pi(n, 0); // Calculate potentials.

    Flow mf(0); // Total flow through the network.
    Cost mincost(0); // Total cost through the network.
    vi p(n, -1); // Path calculation.

    while (1) {
        // Run dijkstra to find path s -> t.
        vector<Cost> d(n, INF);
        p.assign(n, -1);
        d[s] = 0;

        typedef pair<Cost, int> Pair;
        priority_queue<Pair, vector<Pair>, greater<Pair>> q;
        q.push(make_pair(0, s));
        while (!q.empty()) {
            Cost c; int u;
            tie(c, u) = q.top(); q.pop();

            if (c > d[u]) continue;

            // Try forward edge u -> v.
            for (int v : adj_list[u]) {
                Cost next_cost = d[u] + pi[u] - pi[v] + cost[u][v];
                if (flow[u][v] < capacity[u][v] && next_cost < d[v]) {
                    d[v] = next_cost;
                    p[v] = u;
                    q.push(make_pair(d[v], v));
                }
            }

            // Try back edge v -> u.
            for (int v : back_adj_list[u]) {
                Cost next_cost = d[u] + pi[u] - pi[v] - cost[v][u];
                if (flow[v][u] && next_cost < d[v]) {
                    d[v] = next_cost;
                    p[v] = u;
                    q.push(make_pair(d[v], v));
                }
            }
        }

        // Reduce costs.
        for (int u = 0; u < n; u++) {
            if (pi[u] < INF) pi[u] += d[u];
        }

        // Could not find path to t, we're done.
        if (p[t] == -1) break;

        // Find flow
        Flow f = INF;
        for (int v = t; v != s; v = p[v]) {
            int u = p[v];
            // Back edge, can cancel flow amount.
            if (flow[v][u]) f = min(f, flow[v][u]);
            // Normal edge, find residues.
            else f = min(f, capacity[u][v] - flow[u][v]);
        }

        // Update network.
        for (int v = t; v != s; v = p[v]) {
            int u = p[v];
            // Back edge, subtract cost and flow.
            if (flow[v][u]) {
                flow[v][u] -= f;
                mincost -= f * cost[v][u];
            }
            else {
                flow[u][v] += f;
                mincost += f * cost[u][v];
            }
        }
        mf += f;
    }

    return tie(mf, mincost, flow);
}

