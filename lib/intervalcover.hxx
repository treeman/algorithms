#pragma once

template<typename T>
vi cover(pair<T, T> x, vector<pair<T, T> > &intervals) {
    vector<pair<pair<T, T>, int> > xs(intervals.size());
    for (int i = 0; i < (int)intervals.size(); ++i) {
        xs[i].first = intervals[i];
        xs[i].second = i;
    }
    sort(xs.begin(), xs.end());

    vi res;
    T left = x.first;
    int next = 0;
    while (next < (int)xs.size()) {
        int curr = -1;
        T right;
        for (int i = next; i < (int)xs.size(); ++i) {
            pair<T, T> cover = xs[i].first;

            if (cover.first > left) break;
            if (curr == -1 || cover.second >= right) {
                curr = i;
                right = cover.second;
            }
        }

        if (curr == -1) return vi();

        left = xs[curr].first.second;
        res.push_back(xs[curr].second);
        next = curr + 1;
        if (left >= x.second && res.size()) return res;
    }

    return vi();
}

