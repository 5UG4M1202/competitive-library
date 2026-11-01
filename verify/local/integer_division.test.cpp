// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/math/integer_division.hpp"
#include <cassert>
#include <limits>
#include <random>
#include <vector>

static_assert(ceil_div(-7, -3) == 3 && floor_div(-7, -3) == 2);
static_assert(ceil_div(7, -3) == -2 && floor_div(7, -3) == -3);
static_assert(out_div(-7, 3) == -3);

void check(long long a, long long b) {
  using wide = __int128_t;
  if (!b || (a == std::numeric_limits<long long>::min() && b == -1)) return;
  wide x = a, y = b;
  if (y < 0) { x = -x; y = -y; }
  wide q = floor_div(a, b), c = ceil_div(a, b), away = out_div(a, b);
  assert(q * y <= x && x < (q + 1) * y);
  assert((c - 1) * y < x && x <= c * y);
  assert(away == (x < 0 ? q : c));
}

int main() {
  auto lo = std::numeric_limits<long long>::min();
  auto hi = std::numeric_limits<long long>::max();
  std::vector<long long> v{lo, lo + 1, -100, -3, -2, -1, 0, 1, 2, 3, 100, hi - 1, hi};
  for (auto a : v) for (auto b : v) check(a, b);
  for (long long a = -100; a <= 100; ++a)
    for (long long b = -100; b <= 100; ++b) check(a, b);
  std::mt19937_64 rng(537);
  for (int i = 0; i < 100000; ++i) check(static_cast<long long>(rng()), static_cast<long long>(rng()));
}
