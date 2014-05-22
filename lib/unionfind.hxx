#pragma once

// Union find rank ordered with path compression.
// Bonus feature: number of disjoint sets and it tracks size of sets.
class UnionFind {
public:
    UnionFind(int n) {
        num_sets = n;
        rank.assign(n, 0);
        p.assign(n, 0);
        set_size.assign(n, 1);
        for (int i = 0; i < n; ++i) {
            p[i] = i;
        }
    }

    bool same(int a, int b) {
        return find_set(a) == find_set(b);
    }

    void set_union(int a, int b) {
        if (!same(a, b)) {
            --num_sets;
            int x = find_set(a), y = find_set(b);
            if (rank[x] > rank[y]) {
                p[y] = x;
                set_size[x] += set_size[y];
            }
            else {
                p[x] = y;
                set_size[y] += set_size[x];
                if (rank[x] == rank[y]) ++rank[y];
            }
        }
    }

    int num_disjoint() {
        return num_sets;
    }
    int size(int a) {
        return set_size[find_set(a)];
    }
private:
    int find_set(int i) {
        return (p[i] == i) ? i : p[i] = find_set(p[i]);
    }
    vi rank, p, set_size;
    int num_sets;
};

