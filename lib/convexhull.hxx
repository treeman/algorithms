#pragma once

#include "points.hxx"

// Convex hull using Jarvis March.
// Will return a closed convex hull, unless on a single point or empty.
// Can use exact integer points without rounding errors.
template<typename T>
vector<Point<T>> convex_hull(const vector<Point<T>> &points) {
    const int n = points.size();
    if (n < 2) return points;

    vector<Point<T>> ps(points.begin(), points.end());
    sort(ps.begin(), ps.end());

    vector<Point<T>> ch(2 * n);

    // -> take ccw points.
    int l = 0;
    for (int i = 0; i < n; ++i) {
        if (i > 0 && ps[i] == ps[i - 1]) continue;
        while (l >= 2) {
            if (orient(ch[l - 2], ch[l - 1], ps[i]) < 0) break;
            --l;
        }
        ch[l++] = ps[i];
    }

    // Only one point here.
    if (l == 1) {
        ch.resize(1);
        return ch;
    }

    // <- take ccw points
    int r = l;
    for (int i = n - 2; i >= 0; --i) {
        if (ps[i] == ps[i + 1]) continue;
        while (r - l >= 1) {
            if (orient(ch[r - 2], ch[r - 1], ps[i]) < 0) break;
            --r;
        }
        ch[r++] = ps[i];
    }

    // Will close convex hull.
    ch.resize(r);
    return ch;
}

