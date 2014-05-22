#pragma once

#include "modular.hxx"
#include "matrix.hxx"

// Solve a linear recurrence:
// f(i) = a0 + a1 f(i - 1) + a2 f(i - 2) + ... + ak f(i - k)
// Take input a: [a0 a1 .. ak]
// and values of f(i), x: [f(i - k) ... f(i - 2) f(i - 1)]
// can also express as
// x(t) = a0 + a1 x(t - 1) + a2 x(t - 2)
// =>
// x: [x(0) x(1) .. x(t - 1)]
// which is the actual input.
// note: a0 is the constant in the expression.
//
// Ex: Fibonacci
//     f(i) = f(i - 1) + f(i - 2)
//     f(0) = 1, f(1) = 1
//          (can be 0 and 1)
// =>
//    a0 = 0, a1 = 1, a2 = 1
//    x0 = 1, x1 = 1
//
// Return x(t)
// Result is modulo mod as typically result will be very big.
template<typename S>
S linearrecurrence(const vector<S> &a, const vector<S> &x, S t, S mod) {
    if (t == 0) return x[0];
    const int k = a.size() - 1;

    // Default Modular construction uses this modulus.
    Modular<S>::def_mod = mod;

    // a K + 1 elements, a[0] is a constant.
    // x K elements
    Matrix<Modular<S>> F1(k + 1);
    for (int i = 0; i < k; ++i) 
        F1(i, 0) = Modular<S>(x[i], mod);
    F1(k, 0) = Modular<S>(a[0], mod); // Constant is last

    // 1 in (i + 1) element in i:th row, otherwise 0
    // reverse x in last row
    Matrix<Modular<S>> T(k + 1, k + 1);
    for (int i = 0; i < k - 1; ++i) {
        if (i + 1 < k)
            T(i, i + 1) = Modular<S>(1, mod);
    }

    for (int i = 0; i < k; ++i) {
        T(k - 1, k - i - 1) = Modular<S>(a[i + 1], mod);
    }
    // Constant a[0] is the last one, so we have a 1 in the bottom right two lowest.
    T(k - 1, k) = Modular<S>(1, mod);
    T(k, k) = Modular<S>(1, mod);

    // Solution is first row of T^t * F1.
    Matrix<Modular<S>> FN = pow(T, t) * F1;
    return FN(0, 0).x;
}

