// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/math/trial_division.hpp"
#include <cassert>
#include <limits>
#include <vector>

int main() {
  assert(!is_prime(-1) && !is_prime(0) && !is_prime(1) && is_prime(2));
  assert(prime_factor(1).empty());
  assert((divisor(1) == std::vector<long long>{1}));
  assert((sieve_of_era(0) == std::vector<bool>{false}));
  assert((sieve_of_era(1) == std::vector<bool>{false, false}));
  auto sieve = sieve_of_era(3000);
  for (int n = 1; n <= 3000; ++n) {
    std::vector<long long> expected;
    for (int d = 1; d <= n; ++d) if (n % d == 0) expected.push_back(d);
    assert(divisor(n) == expected);
    auto factors = prime_factor(n);
    long long product = 1;
    for (auto [p, exponent] : factors) {
      assert(sieve[p] && exponent > 0);
      while (exponent--) product *= p;
    }
    assert(product == n);
    assert(is_prime(n) == sieve[n]);
  }
  auto factors = prime_factor(std::numeric_limits<long long>::max());
  __int128_t product = 1;
  for (auto [p, exponent] : factors) while (exponent--) product *= p;
  assert(product == std::numeric_limits<long long>::max());
}
