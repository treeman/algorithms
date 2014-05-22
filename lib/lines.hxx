#pragma once

#include "points.hxx"

// Line as
// ax + by + c = 0
// with b = 0 if vertical, otherwise b = 1.
class Line {
public:
    Line(DPoint p1, DPoint p2) {
        // Vertical line
        if (abs(p1.x - p2.x) < EPS) {
            a = 1;
            b = 0;
            c = -p1.x;
        }
        else {
            a = -(p1.y - p2.y) / (p1.x - p2.x);
            b = 1;
            c = -(a * p1.x) - p1.y;
        }
    }
    Line(DPoint p, double slope) {
        a = -slope;
        b = 1;
        c = b * p.y - a * p.x;
    }

    bool operator < (const Line &o) const {
        return tie(a, b, c) < tie(o.a, o.b, o.c);
    }
    bool operator == (const Line &o) const {
        return is_parallel(o) && abs(c - o.c) < EPS;
    }

    bool is_parallel(const Line &o) const {
        return abs(a - o.a) < EPS && abs(b - o.b) < EPS;
    }
    template<typename T>
    bool is_on(const Point<T> &p) const {
        double res = a * (double)p.x + b * (double)p.y + c;
        return abs(res) < EPS;
    }

    // Check if they are parallel first, if so not possible.
    DPoint intersect(const Line &o) {
        DPoint res;
        res.x = (o.b * c - b * o.c) / (o.a * b - a * o.b);
        if (abs(b) > EPS)
            res.y = -(a * res.x + c);
        else
            res.y = -(o.a * res.x + o.c);
        if (abs(res.x) < EPS) res.x = 0;
        if (abs(res.y) < EPS) res.y = 0;
        return res;
    }

    // TODO
    // Return reflection of a point on a line.
    //DPoint reflect() const {

    //}

    // TODO
    //double dist(DPoint p) {

    //}

    // TODO
    // Return closest point?
    //double dist(DPoint p) const {
        //DPoint ap = p - a, ab = b - a;
        //double u = (ap | ab) / (ab | ab);
        //DPoint x = a + (ab * u);
        //return p.dist(x);
    //}

    double a, b, c;
};

// TODO move to line?
// Only makes sense for DPoint's I think?
double dist_to_line(DPoint p, DPoint a, DPoint b) {
    DPoint ap = p - a, ab = b - a;
    double u = (ap | ab) / (ab | ab);
    DPoint c = a + ab * u;
    return p.dist(c);
}

template<typename T>
class LineSegment {
public:
    LineSegment(T x1, T y1, T x2, T y2) : a(x1, y1), b(x2, y2) {
        if (b < a) swap(a, b);
    }

    LineSegment(Point<T> _a, Point<T> _b) : a(_a), b(_b) {
        if (b < a) swap(a, b);
    }

    bool is_point() const { return a == b; }
    bool is_on(Point<T> p) const {
        if (is_point()) return a == p;
        if (!is_collinear(a, b, p)) return false;

        if (p == a || p == b) return true;
        double u = fraction(p);
        if (u < 0.0 || u > 1.0) return false;
        return true;
    }

    // Give fraction of point on line.
    double fraction(Point<T> p) const {
        Point<T> ap = p - a, ab = b - a;
        double u = (double)(ap | ab) / (double)(ab | ab);
        return u;
    }

    double dist(Point<T> p) const {
        if (p == a || p == b) return 0;

        double u = fraction(p);
        if (u < 0.0) return p.dist(a);
        if (u > 1.0) return p.dist(b);
        return dist_to_line(to_dpoint(p), to_dpoint(a), to_dpoint(b));
    }

    double dist(const LineSegment<T> &o) const {
        // Check degenerated cases.
        bool p1 = is_point(), p2 = o.is_point();
        if (p1 && p2) return a.dist(o.a);
        if (p1) return o.dist(a);
        if (p2) return dist(o.a);

        // Simple way of checking for intersection.
        bool o1 = is_ccw(a, b, o.a);
        bool o2 = is_ccw(a, b, o.b);
        bool o3 = is_ccw(o.a, o.b, a);
        bool o4 = is_ccw(o.a, o.b, b);

        if (o1 != o2 && o3 != o4) return 0;

        // Check extreme points.
        double r1 = min(dist(o.a), dist(o.b));
        double r2 = min(o.dist(a), o.dist(b));
        return min(r1, r2);
    }

    // TODO
    //DPoint closest_point(DPoint p) const {

    //}

    // Use DPoint's as an interesection can be in the middle.
    // Use is_on for regular check.
    vector<DPoint> intersect(const Point<T> &p) const {
        if (is_on(p)) {
            vector<DPoint> res = { DPoint(p) };
            return res;
        }
        return vector<DPoint>();
    }
    vector<DPoint> intersect(const LineSegment<T> &o) const {
        // Handle degenerated segments.
        if (o.is_point()) return intersect(o.a);
        if (is_point()) return o.intersect(a);

        // The more complex case when two lines are collinear.
        // They can be inside, partly overlapping, touching or not overlapping.
        if (is_collinear(a, b, o.a) && is_collinear(a, b, o.b)) {
            double fa = o.fraction(a), fb = o.fraction(b);
            if (fa > fb) swap(fa, fb);
            // Not overlapping.
            if (fb < 0.0 || fa > 1.0) return vector<DPoint>();

            // Find min/max points in both.
            DPoint d(o.b - o.a);
            DPoint x(DPoint(o.a) + d * max(fa, 0.0));
            DPoint y(DPoint(o.a) + d * min(fb, 1.0));

            // If they are just touching at one point, just use one.
            if (x == y) {
                vector<DPoint> res = { x };
                return res;
            }

            // Smallest (x, y) first.
            if (y < x) swap(x, y);
            vector<DPoint> res = { x, y };
            return res;
        }

        Line l1(a, b), l2(o.a, o.b);
        // If parallel, no intersection.
        if (l1.is_parallel(l2)) return vector<DPoint>();
        DPoint p = l1.intersect(l2);

        // Handle the segment part of line intersection. Should be on both.
        DPoint da(a), db(b), doa(o.a), dob(o.b);
        if (da <= p && p <= db && doa <= p && p <= dob) {
            vector<DPoint> res = { p };
            return res;
        }

        return vector<DPoint>();
    }

    void print(const char *s = "") const {
        a.print(); printf(" "); b.print(s);
    }

    Point<T> a, b;
};

