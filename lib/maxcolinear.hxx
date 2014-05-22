#include "points.hxx"

template<typename T>
int num_colinear(const vector<Point<T>> &ps) {
    const int n = ps.size();
    if (!n) return 0;

    int res = 0;
    for (int i = 0; i < n; ++i) {
        vector<double> slopes;
        for (int j = i + 1; j < n; ++j) {
            double dx = ps[j].x - ps[i].x, dy = ps[j].y - ps[i].y;
            double s = dx == 0 ? numeric_limits<double>::max() : dy / dx;
            slopes.push_back(s);
        }

        sort(slopes.begin(), slopes.end());
        int curr = 1;
        for (int j = 0; j < (int)slopes.size(); ++j) {
            if (j != (int)slopes.size() - 1 && slopes[j] == slopes[j + 1]) {
                ++curr;
            }
            else {
                curr = 1;
            }
            res = max(res, curr);
        }
    }
    return res + 1;
}

