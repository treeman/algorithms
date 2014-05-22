#pragma once

#include "math.hxx"

template<typename T>
class Point {
public:
    Point() : x(0), y(0) { }
    Point(T _x, T _y) : x(_x), y(_y) { }
    Point(const Point &o) : x(o.x), y(o.y) { }

    bool operator < (const Point &p) const { return tie(x, y) < tie(p.x, p.y); }
    bool operator > (const Point &p) const { return p < *this; }
    bool operator == (const Point &p) const { return x == p.x && y == p.y; }
    bool operator != (const Point &p) const { return !(*this == p); }
    bool operator <= (const Point &p) const { return !(*this > p); }
    bool operator >= (const Point &p) const { return !(*this < p); }

    Point<T> operator * (T s) const { return Point(x * s, y * s); }
    Point<T> operator / (T s) const { return Point(x / s, y / s); }

    Point<T> operator - (const Point &p) const { return Point(x - p.x, y - p.y); }
    Point<T> operator + (const Point &p) const { return Point(x + p.x, y + p.y); }

    T operator | (const Point &p) const { return x * p.x + y * p.y; }
    T cross(const Point &p) const { return x * p.y - y * p.x; }
    double dist(const Point &p) const { return hypot((double)(x - p.x), (double)(y - p.y)); }
    T distsq(const Point &p) const {
        T dx = x - p.x, dy = y - p.y;
        return dx * dx + dy * dy;
    }

    void print(const char *s = "") const {
        cout << "(" << x << ", " << y << ")" << s << flush;
    }

    T x, y;

    static const Point zero;
};

// Return angle aob in radians.
template<typename T>
double angle(const Point<T> &a, const Point<T> &o, const Point<T> &b) {
    auto oa = a - o, ob = b - o;
    return acos((oa | ob) / sqrt((oa | oa) * (ob | ob)));
}

// > 0 ccw, < 0 cw, = 0 collinear.
template<typename T>
T orient(const Point<T> &p, const Point<T> &q, const Point<T> &r) {
    return (p - q).cross(r - q);
}

// Is the angle pqr ccw <=> is r to the left side of line pq?
template<typename T>
bool is_ccw(const Point<T> &p, const Point<T> &q, const Point<T> &r) {
    return orient(p, q, r) > 0;
}

// Are the 3 points on a line?
template<typename T>
bool is_collinear(const Point<T> &p, const Point<T> &q, const Point<T> &r) {
    return orient(p, q, r) == 0;
}

template<typename T>
std::ostream &operator<<(std::ostream &os, Point<T> const &p) {
    return os << p.x << ", " << p.y;
}

template<typename T>
Point<T> make_point(T x, T y) { return Point<T>(x, y); }

typedef Point<double> DPoint;
typedef Point<int> IPoint;

template<typename T>
const Point<T> Point<T>::zero(0, 0);

// Uses epsilon comparisons.
template<>
class Point<double> {
public:
    Point() : x(0), y(0) { }
    Point(double _x, double _y) : x(_x), y(_y) { }
    Point(const Point &o) : x(o.x), y(o.y) { }
    template<typename T>
    Point(const Point<T> &o) : x(o.x), y(o.y) { }

    bool operator < (const Point &p) const {
        if (abs(x - p.x) > EPS) return x < p.x;
        return y < p.y;
    }
    bool operator > (const Point &p) const { return p < *this; }
    bool operator == (const Point &p) const {
        return abs(x - p.x) < EPS && abs(y - p.y) < EPS;
    }
    bool operator != (const Point &p) const { return !(*this == p); }
    bool operator <= (const Point &p) const { return !(*this > p); }
    bool operator >= (const Point &p) const { return !(*this < p); }

    Point<double> operator * (double s) const { return Point(x * s, y * s); }
    Point<double> operator / (double s) const { return Point(x / s, y / s); }

    Point<double> operator - (const Point &p) const { return Point(x - p.x, y - p.y); }
    Point<double> operator + (const Point &p) const { return Point(x + p.x, y + p.y); }

    double operator | (const Point &p) const { return x * p.x + y * p.y; }
    double cross(const Point &p) const { return x * p.y - y * p.x; }
    double dist(const Point &p) const { return hypot(x - p.x, y - p.y); }
    double distsq(const Point &p) const {
        double dx = x - p.x, dy = y - p.y;
        return dx * dx + dy * dy;
    }

    // Rotate ccw with respect to (0, 0) in radians. Only makes sense in double point.
    Point<double> rotate(double rad) {
        return Point<double>(x * cos(rad) - y * sin(rad), x * sin(rad) + y * cos(rad));
    }

    void print(const char *s = "") const {
        cout << "(" << x << ", " << y << ")" << s << flush;
    }

    double x, y;

    static const Point zero;
};

// Are the 3 points on a line?
template<>
bool is_collinear<double>(const DPoint &p, const DPoint &q, const DPoint &r) {
    return abs(orient(p, q, r)) < EPS;
}

template<typename T>
inline DPoint to_dpoint(const Point<T> &p) { return DPoint(p.x, p.y); }

template<>
inline DPoint to_dpoint<double>(const DPoint &p) { return p; }

