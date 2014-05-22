#pragma once

template <typename T, typename It>
vi lis(It begin, It end) {
    const int size = end - begin;
    vector<T> L(size);
    vi lis_index(size), p(size, -1);
    int lis = 0, lis_end = 0;
    for (auto it = begin; it != end; ++it) {
        const int i = it - begin;
        int pos = lower_bound(L.begin(), L.begin() + lis, *it) - L.begin();
        L[pos] = *it;
        lis_index[pos] = i;

        if (pos > 0) p[i] = lis_index[pos - 1];
        if (pos + 1 > lis) {
            lis = pos + 1;
            lis_end = i;
        }
    }

    vi res(lis);
    int x = lis_end;
    for (int i = lis - 1; p[x] >= 0; x = p[x], --i) {
        res[i] = x;
    }
    res[0] = x;
    return res;
}

