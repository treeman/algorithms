#pragma once

class PrimeSieve {
public:
    PrimeSieve(ll n) : lim(n) {
        bs.assign(n + 3, 1);
        bs[0] = bs[1] = 0;
        for (ll i = 2; i <= n; ++i) {
            if (bs[i]) {
                for (ll j = i * i; j <= n; j += i)
                    bs[j] = 0;
                primes.push_back((int)i);
            }
        }
    }

    bool is_prime(ll k) {
        if (k <= lim) return bs[k];
        for (int p : primes)
            if (k % p == 0) return false;
        return true;
    }

    vi get_primes() {
        return primes;
    }
private:
    ll lim;
    vector<bool> bs;
    vi primes;
};

