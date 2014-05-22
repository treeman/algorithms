#pragma once

#include "modular.hxx"

// General chinese remainder.
// Solve a x = 1 mod m
// Take a and m as vectors.
// Return 0 <= x < M, M = lcm(m1, m2)
// If cannot be solved, return <Modular::invalid, 0>
// TODO make general?
template<typename T>
pair<Modular<T>, T> gcrt(T a1, T m1, T a2, T m2) {
    typedef Modular<T> Mod;
    auto fail = make_pair(Mod::invalid, 0);

    if (a2 < a1) {
        swap(a1, a2);
        swap(m1, m2);
    }

    T u, v, g;
    tie(u, v, g) = egcd(m1, m2);
    if (Mod(a1, g) != Mod(a2, g))
        return fail;

    T M = m1 / g * m2;
    Mod x = Mod(a1, M) + Mod(m1, M) * Mod(u, M) * Mod((a2 - a1) / g, M);

    return make_pair(x, M);
}


