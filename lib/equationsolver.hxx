#pragma once

// Solve Ax = b, output x.
// 1: ok
// 0: multiple
// -1: inconsistent
pair<vector<double>, int> gaussian_elimination(
        const vector<vector<double>> &A, const vector<double> &b)
{
    const int n = b.size(); // Assume wellformed dimensions.

    // Augment both A and b into same matrix.
    vector<vector<double>> aug(A.begin(), A.end());
    for (int i = 0; i < n; ++i) {
        aug[i].push_back(b[i]);
    }

    vector<double> x(n, 0);

    // Forward elimintation
    for (int v = 0, h = 0; v < n; ++v) {
        // Find largest column value.
        int l = h;
        for (int r = h + 1; r < n; ++r) {
            if (abs(aug[r][v]) > abs(aug[l][v]))
                l = r;
        }

        if (abs(aug[l][v]) >= EPS) {
            swap(aug[h], aug[l]);

            // Eliminate
            for (int r = h + 1; r < n; ++r) {
                double times = aug[r][v] / aug[h][v];

                for (int c = v; c <= n; ++c) {
                    aug[r][c] -= times * aug[h][c];
                }
            }

            // Continue quest for next.
            ++h;
        }
    }

    // Consistency check
    bool multiple = false, inconsistent = false;
    for (int r = 0; r < n; ++r) {
        bool all_zero = true;
        for (int v = 0; v < n; ++v) {
            if (abs(aug[r][v]) >= EPS) {
                all_zero = false;
                break;
            }
        }

        if (!all_zero) continue;

        if (abs(aug[r][n]) < EPS)
            multiple = true;
        else
            inconsistent = true;
    }

    if (inconsistent) return make_pair(x, -1);
    if (multiple) return make_pair(x, 0);

    // Backward substitution.
    for (int i = n - 1; i >= 0; --i) {
        double t(0);
        for (int k = i + 1; k < n; ++k)
            t += aug[i][k] * x[k];

        double r = aug[i][n] - t;
        x[i] = r / aug[i][i];
    }

    return make_pair(x, 1);
}

