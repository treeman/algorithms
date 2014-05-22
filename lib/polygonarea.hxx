#pragma once

#include "points.hxx"

// Return <area, is_ccw>
template<typename T>
tuple<double, bool> signed_polygon_area(const vector<Point<T>> &polygon) {
    double res = 0;
    for (int i = 0; i < (int)polygon.size() - 1; i++) {
        Point<T> p1 = polygon[i], p2 = polygon[i + 1];
        res += p1.x * p2.y - p2.x * p1.y;
    }
    bool is_ccw = res > 0;
    return make_tuple(abs(res) / 2.0, is_ccw);
}

// Return absolute value of polygon area.
template<typename T>
double polygon_area(const vector<Point<T>> &polygon) {
    double res; bool is_ccw;
    tie(res, is_ccw) = signed_polygon_area<T>(polygon);
    return res;
}

