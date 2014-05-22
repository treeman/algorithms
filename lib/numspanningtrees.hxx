#include "matrix.hxx"

// Kirchoff's theorem.
ll num_spanning_trees(const vector<vi> &adj_list) {
    const int n = adj_list.size();

    // 1.
    // Laplacian matrix:
    // a[i][i] = degree(i)
    // a[i][j] = -1 if adj(i, j)
    // a[i][j] = 0 otherwise
    Matrix<long double> a(n, n, 0);
    for (int u = 0; u < n; ++u) {
        for (int v : adj_list[u]) {
            a(u, v) = -1;
        }
        a(u, u) = adj_list[u].size();
    }

    // 2. delete any row and any column
    Matrix<long double> q = a.shrink(n - 1, n - 1);

    // 3. Determinant is result
    return llroundl(det(q));
}

