#pragma once

#include "gcd.hxx"

//template<typename T>
//T gcd(T a, T b) { return b == T(0) ? a : gcd(b, a % b); }

template<typename T>
class Rational {
public:
    Rational() : x(0), y(1) { }
    Rational(T _x) : x(_x), y(1) { }
    Rational(T _x, T _y) : x(_x), y(_y) {
        if (y < T(0)) {
            x = -x, y = -y;
        }
        T d = gcd(abs(x), abs(y));
        x /= d, y /= d;
    }
    Rational(const Rational<T> &r) : x(r.x), y(r.y) { }

    Rational<T> operator + (const Rational<T> &r) const {
        return Rational<T>(x * r.y + r.x * y, y * r.y);
    }
    Rational<T> operator - (const Rational<T> &r) const {
        return Rational<T>(x * r.y - r.x * y, y * r.y);
    }
    Rational<T> operator * (const Rational<T> &r) const {
        return Rational<T>(x * r.x, y * r.y);
    }
    Rational<T> operator / (const Rational<T> &r) const {
        return Rational<T>(x * r.y, y * r.x);
    }

    bool operator < (const Rational<T> &r) const {
        return x * r.y < r.x * y;
    }
    bool operator > (const Rational<T> &r) const {
        return r < *this;
    }
    bool operator <= (const Rational<T> &r) const {
        return !(r < *this);
    }
    bool operator >= (const Rational<T> &r) const {
        return !(*this < r);
    }
    bool operator == (const Rational<T> &r) const {
        return x == r.x && y == r.y;
    }
    bool operator != (const Rational<T> &r) const {
        return !(*this == r);
    }

    bool is_zero() const { return x == T(0); }

    void print(bool newline = true) {
        if (y == 1) printf("%lld", x);
        else printf("%lld/%lld", x, y);
        if (newline) printf("\n");
    }

    T x, y;

    static const Rational zero;
};

template<typename T>
const Rational<T> Rational<T>::zero(0, 1);

template<typename T>
Rational<T> make_rational(T x, T y) {
    return Rational<T>(x, y);
}

