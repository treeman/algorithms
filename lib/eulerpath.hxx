#pragma once

#include "unionfind.hxx"

// Take a graph as adjecency list. Return edge list of tour or path.
// List is empty if no tour can be found.
vector<ii> eulerian_path(const vector<vi> &adj_list) {
    // We will modify, so make a copy.
    vector<vi> neighbours(adj_list.begin(), adj_list.end());
    vector<ii> fail; // Empty list will be returned in case of failure.

    const int n = neighbours.size();
    UnionFind uf(n); // Track connectivity in an easy manner.

    int num_edges = 0;
    vi in(n, 0), out(n, 0);
    for (int u = 0; u < n; ++u) {
        for (int v : neighbours[u]) {
            ++num_edges;
            ++out[u];
            ++in[v];
            uf.set_union(u, v);
        }
    }

    vi diff;
    int num_zero = 0;
    for (int u = 0; u < n; ++u) {
        if (out[u] != in[u]) {
            diff.push_back(u);
        }
        if (out[u] == 0 && in[u] == 0)
            ++num_zero;
    }

    // Not a connected graph. Don't count zero degree nodes.
    if (uf.num_disjoint() - num_zero != 1) {
        return fail;
    }

    int start = -1;
    // Have path, check in/out diff for both nodes.
    if (diff.size() == 2) {
        int u = diff[0], v = diff[1];
        // u should be 1 more out than in, v should have 1 more in.
        if (in[u] > out[u]) swap(u, v);
        if (out[u] != in[u] + 1) return fail;
        if (in[v] != out[v] + 1) return fail;
        start = u;
    }
    // Tour exists.
    else if (diff.size() == 0) {
        for (int u = 0; u < n; ++u) {
            if (out[u] > 0) {
                start = u;
                break;
            }
        }
    }
    // Path does not exist.
    else {
        return fail;
    }

    vi path;
    stack<int> s;
    int curr = start;
    while (true) {
        // Take next edge.
        if (neighbours[curr].size()) {
            s.push(curr);
            int next = neighbours[curr].back();
            neighbours[curr].pop_back();
            curr = next;
        }
        // No valid edges, add to path.
        else {
            path.push_back(curr);
            if (s.empty()) break;
            curr = s.top(); s.pop();
        }
    }

    // Construct edge list. Path is reversed, make it right.
    vector<ii> edges;
    for (int i = path.size() - 1; i > 0; --i) {
        edges.push_back(ii(path[i], path[i - 1]));
    }
    return edges;
}

