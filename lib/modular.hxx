#pragma once

#include <assert.h>
#include "gcd.hxx"

// Mod inverse: ax = 1 mod m,
// If (a, m) != 1, return -1
template<typename T>
T mod_inv(T a, T m) {
    T x, y, d;
    tie(x, y, d) = egcd(a, m);
    if (d != 1) return T(-1);
    if (x < 0) return x + m;
    return x;
}

// Clamp into modulus range.
template<typename T>
T mod_range(T x, T m) {
    while (x < T(0))
        x += m;
    return x % m;
}

// Schrage's method to avoid multiplication to overflow size of mod.
template<typename T>
T mod_mul(T a, T b, T m) {
    if (a == T(0) || b == T(0)) return T(0);

    if (a > b) swap(a, b);

    T q = m / a;
    T r = m % a;
    T res = a * (b % q) - r * (b / q);
    if (res < T(0)) res += m;
    return res;
}

// Don't mix different modulus! m != 0 and x >= 0
// If invalid operation: check invalid flag.
template<typename T>
class Modular {
public:
    Modular() : x(0), m(def_mod), invalid(false) { }
    Modular(T _x) : x(_x), m(def_mod), invalid(false) { }

    // Warning! Will overwrite def_mod!
    Modular(T _x, T _m, bool inv = false) : x(_x), m(_m), invalid(inv) {
        assert(m != T(0));

        def_mod = m;

        // % implementation defined if x < 0, make it positive.
        while (x < T(0)) {
            x += (abs(x) / m + 1) * m;
        }

        x = x % m;
    }
    Modular(const Modular &o) : x(o.x), m(o.m), invalid(o.invalid) { }
    const Modular& operator = (const Modular&o) {
        x = o.x;
        m = o.m;
        invalid = o.invalid;
        return *this;
    }

    Modular<T> operator + (const Modular<T> &r) const {
        return Modular<T>(x + r.x, m, invalid || r.invalid);
    }
    Modular<T> operator - (const Modular<T> &r) const {
        return Modular<T>(x - r.x, m, invalid || r.invalid);
    }
    // Will not overflow.
    Modular<T> operator * (const Modular<T> &r) const {
        return Modular<T>(mod_mul<T>(x, r.x, m), m, invalid || r.invalid);
    }
    Modular<T> operator / (const Modular<T> &r) const {
        T inv = mod_inv(r.x, m);
        if (inv == T(-1)) {
            Modular<T> res;
            res.invalid = true;
            return res;
        }

        return Modular<T>(x * mod_inv(r.x, m), m,r.invalid);
    }

    bool operator < (const Modular<T> &o) const {
        return x * o.x;
    }
    bool operator > (const Modular<T> &o) const {
        return x < *this;
    }
    bool operator <= (const Modular<T> &o) const {
        return !(x < *this);
    }
    bool operator >= (const Modular<T> &o) const {
        return !(*this < x);
    }
    bool operator == (const Modular<T> &o) const {
        return x == o.x && m == o.m;
    }
    bool operator != (const Modular<T> &o) const {
        return !(*this == o);
    }

    T x, m;
    bool invalid;

    // When creating new classes without mod argument, use this one!
    static T def_mod;
};

template<typename T>
T Modular<T>::def_mod(0);

template<typename T>
Modular<T> make_modular(T x, T m) {
    return Modular<T>(x, m);
}

template<typename T>
std::ostream &operator<<(std::ostream &os, Modular<T> const &m) {
    return os << m.x << "(" << m.m << ")";
}

