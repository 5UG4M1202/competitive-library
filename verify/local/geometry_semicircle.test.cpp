// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/geometry/integer_point.hpp"
#include <algorithm>
#include <cassert>
#include <random>
#include <vector>
using namespace cp::geometry;

// Same contract and algorithm as the documentation example.
int max_open_semicircle(std::vector<IntegerPoint> p) {
  const int n = static_cast<int>(p.size());
  if (n == 0) return 0;
  std::sort(p.begin(), p.end(), ArgumentLess{});
  int best = 0, right = 0;
  for (int left = 0; left < n; ++left) {
    right = std::max(right, left + 1);
    while (right < left + n && cross(p[left], p[right % n]) > 0) ++right;
    best = std::max(best, right - left);
  }
  return best;
}

int main() {
  std::mt19937 random(4298);
  assert(max_open_semicircle({}) == 0);
  for (int round = 0; round < 1000; ++round) {
    std::vector<IntegerPoint> points;
    for (int attempt = 0; attempt < 100 && points.size() < 12; ++attempt) {
      IntegerPoint p{int(random() % 51) - 25, int(random() % 51) - 25};
      if (p.x == 0 && p.y == 0) continue;
      bool unique_line = true;
      for (auto q : points) if (cross(p, q) == 0) unique_line = false;
      if (unique_line) points.push_back(p);
    }
    int expected = 0;
    for (auto p : points) {
      int count = 1;
      for (auto q : points) if (cross(p, q) > 0) ++count;
      expected = std::max(expected, count);
    }
    assert(max_open_semicircle(points) == expected);
  }
}
