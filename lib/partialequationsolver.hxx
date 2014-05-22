#pragma once

// Solve Ax = b, output <x, unique>. Unique says if x is uniquely determined.
// If system is inconsistent return an empty vector.
vector<pair<double, bool>> partial_gaussian_elimination(
        const vector<vector<double>> &A,
        const vector<double> &b)
{
    const int n = b.size(); // Assume wellformed dimensions.

    // Augment both A and b into same matrix.
    vector<vector<double>> aug(A.begin(), A.end());
    for (int i = 0; i < n; ++i) {
        aug[i].push_back(b[i]);
    }

    vector<pair<double, bool>> x(n, make_pair(0, true));
    vi pos(n, -1);

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

            pos[v] = h;

            // Continue quest for next.
            ++h;
        }
        // Could not find v.
        else {
            x[v].second = false;
        }
    }

    // Consistency check
    bool inconsistent = false;
    for (int r = 0; r < n; ++r) {
        bool all_zero = true;
        for (int v = 0; v < n; ++v) {
            if (abs(aug[r][v]) >= EPS) {
                all_zero = false;
                break;
            }
        }

        if (!all_zero) continue;

        if (abs(aug[r][n]) >= EPS)
            inconsistent = true;
    }

    if (inconsistent)
        return vector<pair<double, bool>>();

    // If we can cancel out 2 or more multiple values
    // Check if there is any multiple value left -> this v is also multiple
    // Backward substitution.
    for (int v = n - 1; v >= 0; --v) {
        if (!x[v].second) continue;

        int h = pos[v];

        // Replace equations
        for (int k = v + 1; k < n; ++k) {
            int p = pos[k];
            if (p == -1) continue;
            if (abs(aug[h][k]) < EPS) continue;

            double times = aug[h][k] / aug[p][k];
            for (int c = k; c <= n; ++c) {
                aug[h][c] -= times * aug[p][c];
            }
        }

        // Find value
        double t(0);
        for (int k = v + 1; k < n; ++k) {
            if (!x[k].second && abs(aug[h][k]) > EPS) {
                x[v].second = false;
            }
            t += aug[h][k] * x[k].first;
        }

        double r = aug[h][n] - t;
        x[v].first = r / aug[h][v];
    }

    return x;
}


