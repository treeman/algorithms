#pragma once

#include "modular.hxx"

// Solve a x = 1 mod m
// Take a and m as vectors.
// Return 0 <= x < M with M = fold(ms, *, 1)
template<typename T>
pair<Modular<T>, T> crt(const vector<T> &as, const vector<T> &ms) {
    const int n = as.size();

    T M(1);
    for (T m : ms) M *= m;

    Modular<T> x(0, M);
    for (int i = 0; i < n; ++i) {
        T yi = get<1>(egcd(ms[i], M / ms[i]));
        x = x + Modular<T>(M / ms[i], M) * Modular<T>(yi, M) * Modular<T>(as[i], M);
    }

    return make_pair(x, M);
}

