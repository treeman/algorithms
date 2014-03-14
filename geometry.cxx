/* By: Jonas Hietala
 * Problem statement:
 * First submit:
 * Lesson:
 * Difficulty:
 * Efficiency:
 * Problem solving techniques:
 * Testing and debugging:
 */

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <queue>
#include <deque>
#include <set>
#include <unordered_set>
#include <string>
#include <stack>
#include <bitset>
#include <algorithm>
#include <cstring>
#include <sstream>
#include <complex>
using namespace std;

typedef vector<int> vi;
typedef pair<int, int> ii;
typedef tuple<int, int, int> iii;
typedef long long ll;
typedef unsigned long long ull;
typedef unsigned long nat;

const int INF = 1e9;
const double EPS = 1e-9;
const double PI = acos(-1.0); // alternative (2.0 * acos(0.0))

template<typename T>
class Point {
public:
    Point() : x(0), y(0) { }
    Point(double _x, double _y) : x(_x), y(_y) { }

    bool operator < (const Point &p) const { return tie(x, y) < tie(p.x, p.y); }
    bool operator > (const Point &p) const { return p < *this; }
    bool operator == (const Point &p) const { return x == p.x && y == p.y; }
    bool operator != (const Point &p) const { return !(*this == p); }

    Point<T> operator * (T s) const { return Point(x * s, y * s); }
    Point<T> operator / (T s) const { return Point(x / s, y / s); }

    Point<T> operator - (const Point &p) const { return Point(x - p.x, y - p.y); }
    Point<T> operator + (const Point &p) const { return Point(x + p.x, y + p.y); }

    T operator | (const Point &p) const { return x * p.x + y * p.y; }
    T cross(const Point &p) const { return x * p.y - y * p.x; }
    double dist(const Point &p) const { return hypot(x - p.x, y - p.y); }

    // Is the angle made between the new lines to p and q, ccw?
    bool is_ccw(const Point &q, const Point &r) const {
        const Point p = *this;
        return (p - q).cross(p - r) > 0;
    }

    // Are the 3 points on a line?
    bool is_collinear(const Point &q, const Point &r) const {
        const Point p = *this;
        return abs((p - q).cross(p - r)) < EPS;
    }

    // Return angle made between the new lines to p and q in radians.
    double angle(Point a, Point b) const {
        Point o = *this, oa = o - a, ob = o - b;
        return acos((oa | ob) / sqrt((oa | oa) * (ob | ob)));
    }

    void print(const char *s = "") const {
        cout << "(" << x << ", " << y << ")" << s << flush;
    }

    T x, y;

    static const Point zero;
};

template<typename T>
const Point<T> Point<T>::zero(0, 0);

template<typename T>
Point<T> make_point(T x, T y) { return Point<T>(x, y); }

typedef Point<double> DoublePoint;
typedef Point<int> IntPoint;

typedef Point<double> point;

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
    tie(res, is_ccw) = signed_polygon_area(polygon);
    return res;
}

// TODO
// Return 0 if on border
// -1 if outside
//  1 if inside polygon.
// Polygon can be either convex or concave.
// Assume polygon is closed.
template<typename T>
bool inside_poly(Point<T> p, const vector<Point<T>> &polygon) {
    if (polygon.empty()) return false;

    double angle_sum = 0;
    for (int i = 0; i < (int)polygon.size() - 1; i++) {
        Point<T> p1 = polygon[i], p2 = polygon[i + 1];

        if (p.is_ccw(p1, p2))
            angle_sum += p.angle(p1, p2);
        else
            angle_sum -= p.angle(p1, p2);
    }
    return abs(abs(angle_sum) - 2 * PI) < EPS;
}

// Convex hull pivot check.
template<typename T>
class AngleCompare {
public:
    AngleCompare(Point<T> p) : pivot(p) { }

    bool operator () (const Point<T> &a, const Point<T> &b) const {
        // If collinear, take the closest.
        if (pivot.is_collinear(a, b)) return pivot.dist(a) < pivot.dist(b);

        // Otherwise, smallest tangent angle.
        double ang1 = atan2(a.y - pivot.y, a.x - pivot.x);
        double ang2 = atan2(b.y - pivot.y, b.x - pivot.x);

        return ang1 < ang2;
    }

private:
    Point<T> pivot;
};

// Return convex hull of a list of points.
// Will make sure ch[0] == ch[-1]. Is not needed in input.
// If points is empty, will return empty set.
template<typename T>
vector<Point<T>> convex_hull(const vector<Point<T>> &points) {
    if (points.empty()) return points;

    vector<Point<T>> ps(points.begin(), points.end());
    const int n = ps.size();

    // Here points is the convex hull.
    if (n <= 3) {
        // Make sure ch[0] == ch[-1]
        if (ps.front() != ps.back())
            ps.push_back(ps.front());

        return ps;
    }

    // Step 1. Find p0 as the lowest y, if tie lowest x.
    int p0 = 0;
    for (int i = 1; i < n; ++i) {
        if (ps[i].y < ps[p0].y)
            p0 = i;
        else if (ps[i].y == ps[p0].y && ps[i].x < ps[p0].x)
            p0 = i;
    }

    // Step 2. Sort rest with angles compared to p0.
    swap(ps[0], ps[p0]);
    sort(ps.begin() + 1, ps.end(), AngleCompare<double>(ps[0]));

    // Step 3. ccw tests.
    vector<Point<T>> ch;
    ch.push_back(ps.back());
    ch.push_back(ps[0]);
    ch.push_back(ps[1]);

    for (int i = 2; i < n; ) {
        int last = (int)ch.size() - 1;

        // Left turn, take that point.
        if (ch[last - 1].is_ccw(ch[last], ps[i])) {
            ch.push_back(ps[i]);
            ++i;
        }
        // Right turn, skip the point.
        else {
            ch.pop_back();
        }
    }
    return ch;
}

//typedef point DoublePoint;
typedef vector<DoublePoint> Polygon;
vector<Polygon> kingdoms;

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != -1) {
        Polygon ps;
        for (int i = 0; i < n; ++i) {
            double x, y;
            scanf("%lf %lf", &x, &y);
            ps.push_back(make_point(x, y));
        }
        Polygon ch = convex_hull(ps);
        kingdoms.push_back(ch);

        //printf("kingdom:\n");
        //for (auto p : ch) {
            //p.print("\n");
            //p.print();
        //}
    }

    const int ks = kingdoms.size();
    vi hit(ks, 0);
    double res = 0;
    double x, y;
    while (scanf("%lf %lf", &x, &y) == 2) {
        //printf("=> %.0lf %.0lf?\n", x, y);
        for (int i = 0; i < ks; ++i) {
            if (hit[i]) continue;
            if (inside_poly(make_point(x, y), kingdoms[i])) {
                hit[i] = 1;
                double a = polygon_area(kingdoms[i]);
                res += a;
                //printf("%.1lf %.1lf inside kingdom #%d (%.1lf)\n", x, y, i, a);
            }
        }
    }

    printf("%.2lf\n", res);
    return 0;
}

