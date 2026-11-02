// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/geometry/integer_kd_tree.hpp"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>
#include <numeric>
#include <random>
#include <vector>
using namespace cp::geometry;

struct Sum {
  using value_type = long long;
  static value_type id() { return 0; }
  static value_type op(value_type a, value_type b) { return a + b; }
};
struct Gcd {
  using value_type = int;
  static int id() { return 0; }
  static int op(int a, int b) { return std::gcd(a, b); }
};
struct MinCount {
  struct value_type { int value, count; };
  static value_type id() { return {0, 0}; }
  static value_type op(value_type a, value_type b) {
    if (!a.count) return b;
    if (!b.count) return a;
    if (a.value < b.value) return a;
    if (b.value < a.value) return b;
    return {a.value, a.count + b.count};
  }
};

int main() {
  IntegerKdTree<Sum> empty({}, {});
  assert(empty.prod(-2, 2, -2, 2) == 0);
  const auto lo = std::numeric_limits<std::int64_t>::min();
  const auto hi = std::numeric_limits<std::int64_t>::max();
  IntegerKdTree<Sum> extreme({{hi, hi}, {lo, lo}, {hi, hi}}, {1, 2, 4});
  assert(extreme.prod(IntegerWide(hi), IntegerWide(hi) + 1, IntegerWide(hi), IntegerWide(hi) + 1) == 5);
  assert(extreme.prod(IntegerWide(lo), IntegerWide(hi) + 1, IntegerWide(lo), IntegerWide(hi) + 1) == 7);
  assert(extreme.prod(IntegerWide(lo) - 9, IntegerWide(lo) + 1, IntegerWide(lo) - 9, IntegerWide(lo) + 1) == 2);
  assert(extreme.prod(IntegerWide(hi) + 1, IntegerWide(hi) + 5, IntegerWide(lo), IntegerWide(hi) + 1) == 0);
  assert(extreme.prod(IntegerWide(lo) - 5, IntegerWide(lo), IntegerWide(lo), IntegerWide(hi) + 1) == 0);
  extreme.set(2, 10);
  assert(extreme.prod(IntegerWide(hi), IntegerWide(hi) + 1, IntegerWide(hi), IntegerWide(hi) + 1) == 11);
  assert(extreme.prod(3, 3, -1, 1) == 0);
  IntegerKdTree<Gcd> gcd({{0,0}, {0,0}, {1,0}}, {12, 18, 35});
  assert(gcd.prod(0, 1, 0, 1) == 6);
  gcd.set(0, 15);
  assert(gcd.prod(0, 1, 0, 1) == 3);
  IntegerKdTree<MinCount> mc({{0,0}, {1,0}, {1,0}}, {{3,1}, {2,1}, {2,1}});
  auto min_result = mc.prod(0, 2, 0, 1);
  assert(min_result.value == 2 && min_result.count == 2);
  mc.set(1, {4,1});
  min_result = mc.prod(0, 2, 0, 1);
  assert(min_result.value == 2 && min_result.count == 1);
  assert(mc.prod(2, 3, 0, 1).count == 0);
  std::vector<IntegerPoint> original = {{1, 1}, {3, 3}};
  IntegerKdTree<Sum> owned(original, {4, 5});
  original[0] = {100, 100};
  auto copied = owned;
  copied.set(0, 9);
  assert(owned.prod(0, 2, 0, 2) == 4);
  assert(copied.prod(0, 2, 0, 2) == 9);

  std::mt19937 rng(1031);
  for (int trial = 0; trial < 500; ++trial) {
    const int n = static_cast<int>(rng() % 90);
    std::vector<IntegerPoint> p;
    std::vector<long long> a;
    for (int i = 0; i < n; ++i) {
      p.push_back({static_cast<int>(rng() % 17) - 8, static_cast<int>(rng() % 17) - 8});
      a.push_back(static_cast<int>(rng() % 101) - 50);
    }
    IntegerKdTree<Sum> tree(p, a);
    for (int step = 0; step < 200; ++step) {
      if (n && rng() % 3 == 0) {
        const auto i = rng() % static_cast<unsigned>(n);
        a[i] = static_cast<int>(rng() % 101) - 50;
        tree.set(i, a[i]);
      } else {
        int xl = static_cast<int>(rng() % 23) - 11, xr = static_cast<int>(rng() % 23) - 11;
        int yl = static_cast<int>(rng() % 23) - 11, yr = static_cast<int>(rng() % 23) - 11;
        long long expected = 0;
        for (int i = 0; i < n; ++i)
          if (xl <= p[i].x && p[i].x < xr && yl <= p[i].y && p[i].y < yr) expected += a[i];
        assert(tree.prod(xl, xr, yl, yr) == expected);
      }
    }
  }
}
