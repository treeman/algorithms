#pragma once

template<typename T>
T gcd(T a, T b) { return b == 0 ? a : gcd(b, a % b); }

// Extended gcd, get linear decomposition as well.
template<typename T>
tuple<T, T, T> egcd(T a, T b) {
    if (b == T(0)) {
        return make_tuple(1, 0, a);
    }
    else {
        T x, y, d;
        tie(x, y, d) = egcd(b, a % b);
        x -= a / b * y;
        return make_tuple(y, x, d);
    }
}

