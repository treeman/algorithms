#pragma once

template<typename T>
class Matrix {
public:
    Matrix(int _r, int _c, T fill = T(0)) : r(_r), c(_c) {
        a.assign(r, vector<T>(c, fill));
    }
    // Create a 1-column matrix, treat is a vector.
    Matrix(int _r) : r(_r), c(1) {
        a.assign(r, vector<T>(1, T(0)));
    }
    Matrix(const vector<T> &d) : r(d.size()), c(1) {
        for (int i = 0; i < r; ++i) {
            a[r][0] = d[i];
        }
    }

    Matrix(const vector<vector<T>> &d) : a(d) {
        r = a.front().size();
        c = a.size();
    }

    Matrix(const Matrix &o) : a(o.a), r(o.r), c(o.c) { }

    T &operator()(int i, int j) { return a[i][j]; }

    void operator += (const Matrix &o) {
        for (int i = 0; i < r; ++i)
            for (int j = 0; j < c; ++j)
                a[i][j] = a[i][j] + o.a[i][j];
    }
    void operator -= (const Matrix &o) {
        for (int i = 0; i < r; ++i)
            for (int j = 0; j < c; ++j)
                a[i][j] = a[i][j] - o.a[i][j];
    }
    void operator *= (T k) {
        for (int i = 0; i < r; ++i)
            for (int j = 0; j < c; ++j)
                a[i][j] = a[i][j] * k;
    }

    Matrix<T> operator + (const Matrix &o) const {
        Matrix<T> res(*this);
        res += o;
        return res;
    }
    Matrix<T> operator - (const Matrix &o) const {
        Matrix<T> res(*this);
        res -= o;
        return res;
    }
    Matrix<T> operator * (T k) const {
        Matrix<T> res(*this);
        res *= k;
        return res;
    }

    Matrix<T> operator * (const Matrix &o) const {
        Matrix<T> res(r, o.c);
        for (int i = 0; i < r; ++i) {
            for (int j = 0; j < o.c; ++j) {
                for (int k = 0; k < c; ++k) {
                    res.a[i][j] = res.a[i][j] + a[i][k] * o.a[k][j];
                }
            }
        }
        return res;
    }

    Matrix<T> transpose() const {
        Matrix<T> res(c, r);
        for (int i = 0; i < r; ++i) {
            for (int j = 0; j < r; ++j) {
                res.a[j][i] = a[i][j];
            }
        }
        return res;
    }

    Matrix<T> shrink(int new_r, int new_c) {
        Matrix<T> res(new_r, new_c);
        for (int i = 0; i < new_r; ++i) {
            for (int j = 0; j < new_c; ++j) {
                res.a[i][j] = a[i][j];
            }
        }
        return res;
    }

    void print() {
        for (int i = 0; i < r; ++i) {
            for (int j = 0; j < c; ++j)
                cout << a[i][j] << " ";
            cout << endl << flush;
        }
    }
    vector<vector<T>> a;
    int r, c;
};

template<typename T>
Matrix<T> make_id(int k) {
    Matrix<T> res(k, k);
    for (int i = 0; i < k; ++i) res(i, i) = T(1);
    return res;
}

// Assume square matrix, p >= 0.
// Iterative solution makes for a faster implementation than naive recursive one.
template<typename T, typename S>
Matrix<T> pow(const Matrix<T> &a, S p) {
    Matrix<T> res = make_id<T>(a.r), fact(a);
    while (p) {
        if (p & 1) res = res * fact;
        p >>= 1;
        if (p) fact = fact * fact;
    }
    return res;
}

// Use doubles or similar.
// m.r == m.c > 0
template<typename T>
T det(Matrix<T> m) {
    const int n = m.r;

    // Forward elimination
    for (int k = 0; k < n; ++k) {
        // Find pivot for column k
        T curr_max = m.a[k][k];
        int ind = k;
        for (int i = k + 1; i < n; ++i) {
            if (m.a[i][k] > curr_max) {
                ind = i;
                curr_max = m.a[i][k];
            }
        }

        // Swap row
        swap(m.a[k], m.a[ind]);

        // Eliminate
        for (int i = k + 1; i < n; ++i) {
            for (int j = k + 1; j < n; ++j) {
                m.a[i][j] -= (m.a[k][j] * m.a[i][k]) / m.a[k][k];
            }
            m.a[i][k] = T(0);
        }
    }

    T res = m.a[0][0];
    for (int k = 1; k < n; ++k) {
        res *= m.a[k][k];
    }
    return res;
}


