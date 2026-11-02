// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/geometry/integer_closest_pair.hpp"
#include "../../library/geometry/integer_farthest_pair.hpp"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <random>
#include <vector>
using namespace cp::geometry;

void check(const std::vector<IntegerPoint>& points) {
  auto result = closest_pair(points);
  assert(bool(result) == (points.size() >= 2));
  if (!result) return;
  assert(result->first < points.size() && result->second < points.size());
  assert(result->first != result->second);
  IntegerWide expected = squared_distance(points[0], points[1]);
  for (std::size_t i = 0; i < points.size(); ++i)
    for (std::size_t j = i + 1; j < points.size(); ++j)
      expected = std::min(expected, squared_distance(points[i], points[j]));
  assert(result->distance2 == expected);
  assert(squared_distance(points[result->first], points[result->second]) == expected);
}

int main() {
  check({});
  check({{0, 0}});
  check({{3, 4}, {3, 4}, {3, 4}});
  check({{0, 0}, {2, 0}, {0, 2}, {2, 2}});
  check({{-10, 1}, {-10, 2}, {-10, 5}, {-10, 7}});
  constexpr std::int64_t big = 1000000000000000000LL;
  check({{-big, -big}, {big, big}, {-big, big}});
  assert(squared_distance({-big, -big}, {big, big}) == IntegerWide(8) * big * big);
  auto farthest = farthest_pair({{0, 0}, {3, 4}});
  assert(farthest && farthest->distance2 == 25);
  std::mt19937 rng(2112);
  for (int test = 0; test < 7000; ++test) {
    std::vector<IntegerPoint> p;
    int n = int(rng() % 40);
    for (int i = 0; i < n; ++i)
      p.push_back({int(rng() % 51) - 25, int(rng() % 51) - 25});
    check(p);
  }
  // Many equal x-coordinates and no duplicate points exercise strip handling.
  std::vector<IntegerPoint> line;
  for (int i = 0; i < 100000; ++i) line.push_back({0, i});
  std::shuffle(line.begin(), line.end(), rng);
  auto result = closest_pair(line);
  assert(result && result->distance2 == 1);
  line.push_back(line.front());
  result = closest_pair(line);
  assert(result && result->distance2 == 0 && result->first != result->second);
}
