#pragma once

#include "unionfind.hxx"

// Kruskal's minimum spanning tree. Return the cost of the mst and
// an edge list of type <u, v, weight>.
// Return the empty vector and T(0) if not possible. Take in an edge list tuple<u, v, weight>.
template<typename T>
tuple<T, vector<tuple<int, int, T>>> mst(const vector<tuple<int, int, T>> edges, int n) {
    UnionFind uf(n);
    T weight;
    int u, v;

    // Transform from <u, v, weight> to <weight, u, v> for easy sorting.
    vector<tuple<T, int, int>> ordered_edges;
    for (auto e : edges) {
        tie(u, v, weight) = e;
        ordered_edges.push_back(tie(weight, u, v));
    }
    sort(ordered_edges.begin(), ordered_edges.end());

    T res_weight(0);
    vector<tuple<int, int, T>> chosen;
    for (auto edge : ordered_edges) {
        tie (weight, u, v) = edge;
        if (v < u) swap(u, v); // x < y for all edges.
        if (!uf.same(u, v)) {
            uf.set_union(u, v);
            chosen.push_back(tie(u, v, weight));
            res_weight += weight;
        }
    }

    if (uf.num_disjoint() == 1)
        return make_tuple(res_weight, chosen);
    else
        return make_tuple(T(0), vector<tuple<int, int, T>>());
}

