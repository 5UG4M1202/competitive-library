// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/geometry/integer_convex_contains.hpp"
#include "../../library/geometry/integer_farthest_pair.hpp"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <random>
#include <vector>

using namespace cp::geometry;

// Linear half-plane scan, independent of the logarithmic query.
PointLocation slow_contains(const std::vector<IntegerPoint>& h, IntegerPoint p) {
  if (h.empty()) return PointLocation::outside;
  if (h.size() == 1) return h[0] == p ? PointLocation::boundary : PointLocation::outside;
  if (h.size() == 2) return on_segment(h[0], h[1], p) ? PointLocation::boundary : PointLocation::outside;
  bool boundary = false;
  for (std::size_t i = 0; i < h.size(); ++i) {
    auto value = orientation(h[i], h[(i + 1) % h.size()], p);
    if (value < 0) return PointLocation::outside;
    if (value == 0) boundary = true;
  }
  return boundary ? PointLocation::boundary : PointLocation::inside;
}

int main() {
  assert(!farthest_pair({}));
  assert(!farthest_pair({{2, 2}}));
  auto same = farthest_pair({{4, 4}, {4, 4}, {4, 4}});
  assert(same && same->first != same->second && same->distance2 == 0);
  assert(convex_contains({}, {0, 0}) == PointLocation::outside);
  assert(convex_contains({{0, 0}}, {0, 0}) == PointLocation::boundary);
  assert(convex_contains({{0, 0}, {3, 3}}, {2, 2}) == PointLocation::boundary);
  assert(convex_contains({{0, 0}, {3, 3}}, {4, 4}) == PointLocation::outside);
  constexpr std::int64_t big = 1000000000000000000LL;
  auto wide = farthest_pair({{-big, -big}, {big, big}});
  assert(wide && wide->distance2 == IntegerWide(8) * big * big);
  std::mt19937 rng(7301);
  for (int test = 0; test < 5000; ++test) {
    std::vector<IntegerPoint> points;
    int n = int(rng() % 20);
    for (int i = 0; i < n; ++i)
      points.push_back({int(rng() % 19) - 9, int(rng() % 19) - 9});
    auto ans = farthest_pair(points);
    assert(bool(ans) == (points.size() >= 2));
    if (ans) {
      IntegerWide expected = 0;
      for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j)
        expected = std::max(expected, squared_distance(points[i], points[j]));
      assert(ans->first < points.size() && ans->second < points.size());
      assert(ans->first != ans->second && ans->distance2 == expected);
      assert(squared_distance(points[ans->first], points[ans->second]) == expected);
    }
    auto hull = convex_hull(points);
    // The algorithm accepts any cyclic rotation of a strict convex hull.
    if (!hull.empty()) std::rotate(hull.begin(), hull.begin() + rng() % hull.size(), hull.end());
    for (int k = 0; k < 50; ++k) {
      IntegerPoint p{int(rng() % 31) - 15, int(rng() % 31) - 15};
      assert(convex_contains(hull, p) == slow_contains(hull, p));
    }
    for (auto p : hull) assert(convex_contains(hull, p) == PointLocation::boundary);
  }
}
