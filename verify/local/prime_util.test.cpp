// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
// Intended correctness for valid inputs and characterization of x < 2.
#include "../../library/math/prime_util.hpp"
#include <cassert>
#include <map>
#include <vector>

int main() {
  PrimeUtil primes(2000);
  assert(!primes.is_prime(-1) && !primes.is_prime(0) && !primes.is_prime(1));
  assert(primes.factor(1).empty());
  assert((primes.divisor(1) == std::vector<int>{1}));
  // Legacy characterization, not mathematical factorization for nonpositive x.
  assert(primes.factor(0).empty());
  assert((primes.divisor(0) == std::vector<int>{1}));
  for (int x = 1; x <= 2000; ++x) {
    std::vector<int> divisors;
    for (int d = 1; d <= x; ++d) if (x % d == 0) divisors.push_back(d);
    assert(primes.divisor(x) == divisors);
    assert(primes.is_prime(x) == (divisors.size() == 2));
    auto factors = primes.factor(x);
    int product = 1;
    for (auto [p, exponent] : factors) {
      assert(primes.is_prime(p) && exponent > 0);
      for (int i = 0; i < exponent; ++i) product *= p;
    }
    assert(product == x);
    auto unsorted = primes.divisor(x, false);
    std::sort(unsorted.begin(), unsorted.end());
    assert(unsorted == divisors);
  }
}
