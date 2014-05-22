#pragma once

// Knuth Morris Pratt string matching. Return a vector of matching positions.
vi find(string pattern, string text) {
    // Initialize back table.
    vi back(text.size());
    back[0] = -1;
    int i = 0, j = -1;
    while (i < (int)pattern.size()) {
        while (j >= 0 && pattern[i] != pattern[j]) {
            j = back[j];
        }
        ++i; ++j;
        back[i] = j;
    }

    // Search for positions, use back table to never match twice.
    vi positions;
    i = 0, j = 0;
    while (i < (int)text.size()) {
        while (j >= 0 && text[i] != pattern[j]) {
            j = back[j];
        }
        ++i; ++j;
        if (j == (int)pattern.size()) {
            positions.push_back(i - j);
            j = back[j];
        }
    }
    return positions;
}


