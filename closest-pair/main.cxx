const double INF = 1e9;

struct Point {
    Point(double _x, double _y) : x(_x), y(_y) { }

    bool operator < (const Point &p) const { return tie(x, y) < tie(p.x, p.y); }
    double x, y;
};

double distsq(const Point &a, const Point &b) {
    double dx = a.x - b.x, dy = a.y - b.y;
    return dx * dx + dy * dy;
}

double closestsq(int l, int r, const vector<Point> &points) {
    if (l == r) return INF;
    if (l + 1 == r) return distsq(points[l], points[r]);

    // Divide into 2 equal sets, by a horisontal line, recurse.
    int m = l + (r - l) / 2;
    double d = min(closestsq(l, m, points), closestsq(m + 1, r, points));

    // Only consider points from both sets where dx <= d.
    vi box;
    for (int i = l; i <= r; ++i) {
        if (abs(points[m].x - points[i].x) < d + EPS) {
            box.push_back(i);
        }
    }
    for (int i = 0; i < (int)box.size(); ++i) {
        for (int j = i + 1; j < (int)box.size(); ++j) {
            int a = box[i], b = box[j];

            double dis = distsq(points[a], points[b]);
            if (d - dis > EPS) {
                d = dis;
            }
        }
    }
    return d;
}

double closest_pair(const vector<Point> &points) {
    if (points.size() <= 1) return INF;

    vector<Point> ps(points.begin(), points.end());
    sort(ps.begin(), ps.end());

    double res = closestsq(0, ps.size() - 1, ps);
    if (abs(res - INF) < EPS) return INF;
    return sqrt(res);
}

