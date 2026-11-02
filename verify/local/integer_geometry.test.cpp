// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/geometry/integer_convex_hull.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <random>
#include <vector>

using cp::geometry::IntegerPoint;
using cp::geometry::ArgumentLess;
using cp::geometry::orientation;
using cp::geometry::convex_hull;

static bool on_segment(IntegerPoint a, IntegerPoint b, IntegerPoint p) {
  return orientation(a, b, p) == 0 &&
      std::min(a.x, b.x) <= p.x && p.x <= std::max(a.x, b.x) &&
      std::min(a.y, b.y) <= p.y && p.y <= std::max(a.y, b.y);
}

int main() {
  using namespace cp::geometry;
  assert(dot({2, 3}, {-1, 4}) == 10);
  assert(cross({2, 3}, {-1, 4}) == 11);
  assert(norm2({3, 4}) == 25);
  assert(same_direction({2, 4}, {1, 2}));
  assert(opposite_direction({2, 4}, {-1, -2}));
  assert(!same_direction({0, 0}, {1, 0}));
  assert(!opposite_direction({0, 0}, {1, 0}));
  constexpr std::int64_t large = 1000000000000000000LL;
  assert(orientation({-large, -large}, {large, -large}, {-large, large}) > 0);
  std::vector<IntegerPoint> grid;
  for (int x = -3; x <= 3; ++x) for (int y = -3; y <= 3; ++y) grid.push_back({x, y});
  ArgumentLess less;
  for (auto a : grid) for (auto b : grid) {
    const long double aa = (a.x == 0 && a.y == 0) ? 0 : std::atan2((long double)a.y, (long double)a.x);
    const long double bb = (b.x == 0 && b.y == 0) ? 0 : std::atan2((long double)b.y, (long double)b.x);
    if (std::abs(aa - bb) > 1e-12L) assert(less(a, b) == (aa < bb));
    else assert(!less(a, b) && !less(b, a));
    for (auto c : grid) {
      if (less(a, b) && less(b, c)) assert(less(a, c));
      if (!less(a, b) && !less(b, a) && !less(b, c) && !less(c, b))
        assert(!less(a, c) && !less(c, a));
    }
  }

  assert(convex_hull({}).empty());
  std::vector<IntegerPoint> line = {{2, 2}, {0, 0}, {1, 1}, {1, 1}};
  assert((convex_hull(line) == std::vector<IntegerPoint>{{0, 0}, {2, 2}}));
  assert((convex_hull(line, true) == std::vector<IntegerPoint>{{0, 0}, {1, 1}, {2, 2}}));
  std::mt19937 gen(341);
  for (int trial = 0; trial < 2000; ++trial) {
    std::vector<IntegerPoint> pts;
    for (int i = 0, n = 3 + int(gen() % 7); i < n; ++i)
      pts.push_back({int(gen() % 9) - 4, int(gen() % 9) - 4});
    auto unique = pts;
    std::sort(unique.begin(), unique.end());
    unique.erase(std::unique(unique.begin(), unique.end()), unique.end());
    auto hull = convex_hull(pts), boundary = convex_hull(pts, true);
    auto sorted = hull;
    std::sort(sorted.begin(), sorted.end());
    assert(std::unique(sorted.begin(), sorted.end()) == sorted.end());
    for (auto p : hull) assert(std::binary_search(unique.begin(), unique.end(), p));
    if (hull.size() < 3) {
      assert(boundary == unique);
      continue;
    }
    for (std::size_t i = 0; i < hull.size(); ++i) {
      auto a = hull[i], b = hull[(i + 1) % hull.size()];
      assert(orientation(a, b, hull[(i + 2) % hull.size()]) > 0);
      for (auto p : pts) assert(orientation(a, b, p) >= 0);
    }
    for (auto p : unique) {
      bool expected = false;
      for (auto a : unique) for (auto b : unique) {
        if (a == b || !on_segment(a, b, p)) continue;
        bool positive = false, negative = false;
        for (auto q : unique) {
          positive |= orientation(a, b, q) > 0;
          negative |= orientation(a, b, q) < 0;
        }
        if (!positive || !negative) expected = true;
      }
      assert((std::find(boundary.begin(), boundary.end(), p) != boundary.end()) == expected);
    }
    auto ordered = boundary;
    std::sort(ordered.begin(), ordered.end());
    assert(std::unique(ordered.begin(), ordered.end()) == ordered.end());
    for (std::size_t i = 0; i < boundary.size(); ++i)
      assert(orientation(boundary[i], boundary[(i + 1) % boundary.size()],
                         boundary[(i + 2) % boundary.size()]) >= 0);
  }
}
