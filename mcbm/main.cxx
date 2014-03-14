
vi match, vis;
int aug(int l, const vector<vi> &adj_list) {
    if (vis[l]) return 0;
    vis[l] = 1;
    for (int r : adj_list[l]) {
        if (match[r] == -1 || aug(match[r], adj_list)) {
            match[r] = l;
            return 1;
        }
    }
    return 0;
}

int mcbm(const vector<vi> &adj_list) {
    const int n = adj_list.size();

    int res = 0; // result
    match.assign(n, -1);
    for (int l = 0; l < n; ++l) {
        vis.assign(n, 0);
        res += aug(l, adj_list);
    }
    return res;
}

