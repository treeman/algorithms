#pragma once

// Return <start, length> of cycle defined by f and x0.
// Define a function f before.
template<typename T>
pair<T, T> cycle_finding(T x0) {
    T tortoise = f(x0), hare = f(f(x0));
    // 1st phase, hare 2x speed.
    while (tortoise != hare) {
        tortoise = f(tortoise);
        hare = f(f(hare));
    }

    // 2nd phase, find start of cycle.
    T start = 0; hare = x0;
    while (tortoise != hare) {
        tortoise = f(tortoise);
        hare = f(hare);
        ++start;
    }

    // 3rd phase, find cycle length. Hare moves, tortoise still.
    T len = 1; hare = f(tortoise);
    while (tortoise != hare) {
        hare = f(hare);
        ++len;
    }

    return make_pair(start, len);
}

