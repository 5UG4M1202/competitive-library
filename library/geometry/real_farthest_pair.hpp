#pragma once

#include "real_convex_hull.hpp"
#include <algorithm>
#include <cstddef>
#include <optional>
#include <utility>
#include <vector>

namespace cp::geometry {

struct RealFarthestPair {
  std::size_t first, second;  // Distinct indices into the input.
  long double distance2;
};

// Ideal real arithmetic: O(n log n). EPS and rounding can invalidate the
// strict-hull assumptions; ties are arbitrary.
inline std::optional<RealFarthestPair> farthest_pair(const std::vector<RealPoint>& points,
                                                      const Geometry2D& geo = Geometry2D{}) {
  if (points.size() < 2) return std::nullopt;
  auto hull = convex_hull(points, false, geo);
  if (hull.size() == 1) return RealFarthestPair{0, 1, 0};

  std::vector<std::pair<RealPoint, std::size_t>> index;
  index.reserve(points.size());
  for (std::size_t i = 0; i < points.size(); ++i) index.emplace_back(points[i], i);
  std::sort(index.begin(), index.end());
  const auto original_index = [&](RealPoint p) {
    auto it = std::lower_bound(index.begin(), index.end(), std::pair{p, std::size_t{0}});
    return it->second;
  };

  std::size_t best_a = 0, best_b = 1;
  long double best = squared_distance(hull[0], hull[1]);
  const auto consider = [&](std::size_t a, std::size_t b) {
    auto d = squared_distance(hull[a], hull[b]);
    if (d > best) { best = d; best_a = a; best_b = b; }
  };
  if (hull.size() > 2) {
    const std::size_t n = hull.size();
    std::size_t j = 1;
    for (std::size_t i = 0; i < n; ++i) {
      const std::size_t next = (i + 1) % n;
      while (cross(hull[next] - hull[i], hull[(j + 1) % n] - hull[i]) >
             cross(hull[next] - hull[i], hull[j] - hull[i])) j = (j + 1) % n;
      consider(i, j);
      consider(next, j);
      consider(i, (j + 1) % n);
      consider(next, (j + 1) % n);
    }
  }
  return RealFarthestPair{original_index(hull[best_a]), original_index(hull[best_b]), best};
}

}  // namespace cp::geometry
