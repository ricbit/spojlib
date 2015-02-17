#include <ext/numeric>
#include <functional>
#include <cmath>
#include <vector>

//reserve sieve getprimes factorize sum_of_divisors gcd totient

template<typename PrimeCallback>
std::vector<bool> sieve(int maxprime, PrimeCallback callback) {
  std::vector<bool> primes(1 + maxprime, true);
  primes[0] = primes[1] = false;
  int i = 2;
  int maxp = static_cast<int>(sqrt(maxprime));
  for (; i <= maxp; i++) {
    if (primes[i]) {
      callback(i);
      for (int j = i * i; j <= maxprime; j += i) {
        primes[j] = false;
      }
    }
  }
  if (i % 2 == 0) i++;
  for (; i <= maxprime; i += 2) {
    if (primes[i]) {
      callback(i);
    }
  }
  return primes;
}

std::vector<bool> sieve(int maxprime) {
  return sieve(maxprime, [](int i){});
}

std::vector<int> getprimes(int maxprime) {
  std::vector<int> primes;
  sieve(maxprime, [&primes](int prime) {
    primes.push_back(prime);
  });
  return primes;
}


void divmod(int a, int b, int& div, int& mod) {
  asm (
    "idivl %%esi\n\t"
    : "=d" (mod), "=a" (div)
    : "d" (0), "a" (a), "S" (b)
    : "cc"
  );
}

template <typename FactorCallback, typename T>
void factorize(T number, const std::vector<int>& primes,
               FactorCallback factor) {
  int maxp = static_cast<int>(sqrt(number));
  for (int i = 0; primes[i] <= maxp; i++) {
    if (number % primes[i] == 0) {
      int fac = 1;
      number /= primes[i];
      while (number % primes[i] == 0) {
        fac++;
        number /= primes[i];
      }
      factor(primes[i], fac);
    }
  }
  if (number > 1) {
    factor(number, 1);
  }
}

template<typename T>
T power(T value, int n) {
  if (n == 0) {
    return T(1);
  } else {
    T half = power(value, n / 2);
    if (n % 2 == 0) {
      return half * half;
    } else {
      return half * half * value;
    }
  }
}

template<typename T>
T sum_of_divisors(int n, const std::vector<int>& primes) {
  T ans(1);
  factorize(n, primes, [&ans](int prime, int n) {
    ans *= (power(prime, n + 1) - 1) / (prime - 1);
  });
  return ans;
}

template<typename T>
T totient(int n, const std::vector<int>& primes) {
  T ans(1);
  factorize(n, primes, [&ans](int prime, int n) {
    T exp = power(prime, n);
    ans *= exp - exp / prime;
  });
  return ans;
}

template<typename T>
T gcd(T a, T b) {
  if (b == 0) {
    return a;
  } else {
    return gcd(b, a % b);
  }
}
