#pragma once

// Define MAX_ITEMS and MAX_WEIGHT before!

// Memoization for knapsack.
// Much faster to have statically allocated if we're making multiple runs.
int memo[MAX_ITEMS][MAX_WEIGHT];

// Iterators contains items with <value, weight>.
// Returns indexes/offsets to the chosen items.
template<typename It>
vi knapsack(int capacity, It begin, It end) {
    const int n = end - begin;

    for (int i = 0; i <= n; ++i) memo[i][0] = 0;
    for (int w = 0; w <= capacity; ++w) memo[0][w] = 0;

    for (int id = 1; id <= n; ++id) {
        for (int w = 1; w <= capacity; ++w) {
            const int value = (begin + id - 1)->first;
            const int weight = (begin + id - 1)->second;
            if (weight > w) memo[id][w] = memo[id - 1][w];
            else memo[id][w] = max(memo[id - 1][w], value + memo[id - 1][w - weight]);
        }
    }

    // Backtrack and find taken items.
    vi res;
    int weight = capacity;
    for (int id = n; id >= 1; --id) {
        if (memo[id][weight] != memo[id - 1][weight]) {
            weight -= (begin + id - 1)->second;
            res.push_back(id - 1);
        }
    }

    return res;
}


