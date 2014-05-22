
// Used to answer dynamic range sum queries.
template<typename T>
class FenwickTree {
public:
    FenwickTree(int n) { bit.assign(n + 1, T(0)); }

    // sum 0 <= x <= b
    T sum(int b) {
        if (b < 0) return T(0);
        T res = T(0);
        for (; b > 0; b -= (b & (-b)))
            res += bit[b];
        return res;
    }

    // sum a <= x <= b
    T sum(int a, int b) {
        return sum(b) - sum(a - 1);
    }

    // a[i] += T
    void add(int i, T val) {
        if (!i) {
            bit[0] += val;
        }
        else {
            for (; i < (int)bit.size(); i += (i & (-i)))
                bit[i] += val;
        }
    }
private:
    vector<T> bit;
};
