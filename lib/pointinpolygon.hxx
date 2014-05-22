#pragma once

#include "points.hxx"
#include "lines.hxx"

// Return 0 if on border
// -1 if outside
//  1 if inside polygon.
// Polygon can be either convex or concave.
// Assume polygon is closed.
template<typename T>
int inside_poly(Point<T> p, const vector<Point<T>> &polygon) {
    if (polygon.empty()) return false;

    const int n = polygon.size();
    for (int i = 0; i < n - 1; ++i) {
        if (LineSegment<T>(polygon[i], polygon[i + 1]).is_on(p)) return 0;
    }

    // Raycast to the right, if line intersects ray at p.y, count it.
    // If odd lines met, point is inside.
    bool in = false;
    for (int i = 0; i < n - 1; ++i) {
        auto p1 = polygon[i], p2 = polygon[i + 1];
        if ((min(p1.y, p2.y) <= p.y && p.y < max(p1.y, p2.y)) &&
            ((double)p.x < (double)((p2.x - p1.x) * (p.y - p1.y)) / (double)(p2.y - p1.y) + (double)p1.x + EPS))
        {
            in = !in;
        }
    }
    return in ? 1 : -1;
}

