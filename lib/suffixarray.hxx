#pragma once

// Suffix Array constructed in O(n log n) time.
class SuffixArray {
public:
    // Supports adding an unique end character, for more complex use.
    SuffixArray(const string &s, char end = 0) {
        if (end == 0) construct(s);
        else construct(s + end);
    }

    // Return the start position of the i:th smallest suffix.
    int get_suffix(int i) { return suffixes[i]; }
private:
    // The fast version!
    void construct(const string &s) {
        txt = s;
        rank.resize(txt.size());
        suffixes.resize(txt.size());
        vi tmp(txt.size());
        const int n = txt.size();

        for (int i = 0; i < n; ++i) {
            rank[i] = s[i];
            suffixes[i] = i;
        }

        // Sort log n times.
        for (int k = 1; k < n; k <<= 1) {
            // Radix sort on rank on position 0 and k of all prefixes.
            counting_sort(k);
            counting_sort(0);

            // Re-rank from rank 0. If same rank pair, same rank, otherwise +1.
            int r = 0;
            tmp[suffixes[0]] = r;
            for (int i = 1; i < n; ++i) {
                const int curr_rank = suffixes[i], prev_rank = suffixes[i - 1];
                ii curr(rank[curr_rank], rank[curr_rank + k]);
                ii prev(rank[prev_rank], rank[prev_rank + k]);

                if (prev == curr) tmp[suffixes[i]] = r;
                else tmp[suffixes[i]] = ++r;
            }
            rank.assign(tmp.begin(), tmp.end());

            // When all suffixes gain a new rank, nothing moved, and we can stop.
            if (rank[suffixes[txt.size() - 1]] == n - 1) break;
        }
    }

    // Linear sort on rank[suffixes[i] + k].
    // Update result in suffixes.
    void counting_sort(int k) {
        vi c(max((size_t)256, txt.size()), 0); // Use c[0] if i + k >= n.

        // Count freq of each integer rank.
        for (int i = 0; i < (int)txt.size(); ++i) {
            if (i + k >= (int)txt.size()) c[0]++;
            else c[rank[i + k]]++;
        }

        // Add in counting offset.
        int sum = 0;
        for (int i = 0; i < (int)c.size(); ++i) {
            int tmp = c[i];
            c[i] = sum;
            sum += tmp;
        }

        // Update suffixes.
        vi tmp(txt.size());
        for (int i = 0; i < (int)txt.size(); ++i) {
            if (suffixes[i] + k >= (int)txt.size())
                tmp[c[0]++] = suffixes[i];
            else
                tmp[c[rank[suffixes[i] + k]]++] = suffixes[i];
        }
        // Store result.
        suffixes.assign(tmp.begin(), tmp.end());
    }

    string txt;
    vi rank, suffixes;
};

