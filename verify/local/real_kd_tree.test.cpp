// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/geometry/real_kd_tree.hpp"
#include <cassert>
#include <cmath>
#include <limits>
#include <random>
#include <vector>
using namespace cp::geometry;
struct Sum {
  using value_type = long long;
  static value_type id() { return 0; }
  static value_type op(value_type a, value_type b) { return a + b; }
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
  const double inf = std::numeric_limits<double>::infinity();
  const double hi = std::numeric_limits<double>::max();
  const double next = std::nextafter(1.0, inf);
  const long double long_next = std::nextafter(1.0L, std::numeric_limits<long double>::infinity());
  RealKdTree<Sum> precision({{1.0L,0}, {long_next,0}}, {2,3});
  assert(precision.prod(1.0L,long_next,-1,1) == 2);
  RealKdTree<Sum> e({}, {});
  assert(e.prod(-inf, inf, -inf, inf) == 0);
  std::vector<RealPoint> p = {{-0.0, 0.0}, {0.0, -0.0}, {1, 1}, {next, 1}, {hi, hi}};
  RealKdTree<Sum> tree(p, {2, 3, 5, 7, 11});
  assert(tree.prod(-inf, inf, -inf, inf) == 28);
  assert(tree.prod(0.0, next, 0.0, next) == 10);
  assert(tree.prod(1.0, next, 1.0, next) == 5);
  assert(tree.prod(hi, inf, hi, inf) == 11);
  assert(tree.prod(-0.0, 0.0, -inf, inf) == 0);
  p[2] = {20, 20};
  auto copy = tree;
  copy.set(2, 13);
  assert(tree.prod(1.0, next, 1.0, next) == 5);
  assert(copy.prod(1.0, next, 1.0, next) == 13);
  RealKdTree<MinCount> mc({{0.25, 0}, {0.5, 0}, {0.5, 0}}, {{3, 1}, {2, 1}, {2, 1}});
  auto minimum = mc.prod(0, 1, -1, 1);
  assert(minimum.value == 2 && minimum.count == 2);
  mc.set(1, {4, 1});
  minimum = mc.prod(0, 1, -1, 1);
  assert(minimum.value == 2 && minimum.count == 1);
  std::mt19937 rng(1200);
  for (int rep = 0; rep < 400; ++rep) {
    int n = static_cast<int>(rng() % 70);
    std::vector<RealPoint> points;
    std::vector<long long> values;
    for (int i = 0; i < n; ++i) {
      points.push_back({(static_cast<int>(rng() % 31) - 15) / 8.0,
                        (static_cast<int>(rng() % 31) - 15) / 8.0});
      values.push_back(static_cast<int>(rng() % 101) - 50);
    }
    RealKdTree<Sum> kd(points, values);
    for (int j = 0; j < 160; ++j) {
      if (n && rng() % 3 == 0) {
        auto id = rng() % static_cast<unsigned>(n);
        values[id] = static_cast<int>(rng() % 101) - 50;
        kd.set(id, values[id]);
      } else {
        auto coord = [&] { return (static_cast<int>(rng() % 39) - 19) / 8.0; };
        double xl = coord(), xr = coord(), yl = coord(), yr = coord();
        long long expected = 0;
        for (int i = 0; i < n; ++i)
          if (xl <= points[i].x && points[i].x < xr && yl <= points[i].y && points[i].y < yr)
            expected += values[i];
        assert(kd.prod(xl, xr, yl, yr) == expected);
      }
    }
  }
}
