#pragma once

#include "points.hxx"

// Return distance squared and the closest points.
// Will return INF if called with less than 2 points.
template<typename T>
tuple<T, int, int> closestsq(
        const vector<Point<T>> &ps,
        const vi &xs, // Indexes into ps, sorted by x.
        const vi &ys) // Indexes into ps, sorted by y.
{
    const int n = xs.size();

    if (n <= 1) return make_tuple(INF, -1, -1);
    if (n == 2) return make_tuple(ps[xs[0]].distsq(ps[xs[1]]), xs[0], xs[1]);

    // Divide into 2 equal sets, by a horisontal line, recurse.
    int m = n / 2;
    Point<T> mid = ps[xs[m]];

    vi lxs, rxs;
    vi lys, rys, same_y;
    for (int i = 0; i < n; ++i) {
        if (i < m) lxs.push_back(xs[i]);
        else rxs.push_back(xs[i]);

        // If we have the same x coordinates, handle them separately.
        if (ps[ys[i]].x == mid.x) same_y.push_back(ys[i]);
        else if (ps[ys[i]].x < mid.x) lys.push_back(ys[i]);
        else rys.push_back(ys[i]);
    }

    // Handle same x coordinates and give balanced partitions.
    for (int k : same_y) {
        if (lys.size() < lxs.size()) lys.push_back(k);
        else rys.push_back(k);
    }

    // Recurse and solve smaller problems.
    T d1, d2; int a1, a2, b1, b2;
    tie(d1, a1, a2) = closestsq(ps, lxs, lys);
    tie(d2, b1, b2) = closestsq(ps, rxs, rys);

    T d = d1;
    int p1 = a1, p2 = a2;
    if (d2 < d) {
        d = d2;
        p1 = b1; p2 = b2;
    }

    // Find points within d of mid point.
    vi box;
    for (int i = 0; i < n; ++i) {
        if (abs(ps[ys[i]].x - mid.x) < d) {
            box.push_back(ys[i]);
        }
    }

    // Check all points inside a strip 2d wide.
    for (int i = 0; i < (int)box.size(); ++i) {
        const Point<T> a = ps[box[i]];
        for (int j = i + 1; j < (int)box.size(); ++j) {
            const Point<T> b = ps[box[j]];

            // Makes it so loop will run <= 6 times!
            if (abs(b.y - a.y) >= d) break;

            T dis = a.distsq(b);
            if (dis < d) {
                d = dis;
                p1 = box[i];
                p2 = box[j];
            }
        }
    }

    return tie(d, p1, p2);
}

// Sorting on indexes into points.
template<typename T>
class Ycmp {
public:
    Ycmp(const vector<Point<T>> &_ps) : ps(_ps) { }

    bool operator()(const int i, const int j) const {
        return ps[i].y < ps[j].y;
    }

    const vector<Point<T>> &ps;
};

template<typename T>
class Xcmp {
public:
    Xcmp(const vector<Point<T>> &_ps) : ps(_ps) { }

    bool operator()(const int i, const int j) const {
        return ps[i].x < ps[j].x;
    }

    const vector<Point<T>> &ps;
};

// Return distance squared and the closest points.
// Will return INF if called with less than 2 points.
template<typename T>
tuple<T, int, int> closest_pair(const vector<Point<T>> &points) {
    const int n = points.size();

    if (n <= 1) return make_tuple(INF, -1, -1);

    vi xs(n), ys(n);
    for (int i = 0; i < n; ++i) xs[i] = ys[i] = i;

    sort(xs.begin(), xs.end(), Xcmp<T>(points)); // xs is sorted on x
    sort(ys.begin(), ys.end(), Ycmp<T>(points)); // ys sorted on y

    T res; int i, j;
    tie(res, i, j) = closestsq(points, xs, ys);

    if (points[j] < points[i]) swap(i, j);
    return make_tuple(res, i, j);
}

// Complete search in O(n^2) time.
// Return distance squared and indexes to a pair of closest points.
template<typename T>
tuple<T, int, int> closest_pair_slow(const vector<Point<T>> &ps) {
    const int n = ps.size();

    if (n <= 1) return make_tuple(INF, -1, -1);

    T d = INF;
    int p1 = -1, p2 = -1;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            T dis = ps[i].distsq(ps[j]);
            if (dis < d) {
                d = dis;
                p1 = i; p2 = j;
            }
        }
    }

    if (ps[p2] < ps[p1]) swap(p1, p2);
    return tie(d, p1, p2);
}

