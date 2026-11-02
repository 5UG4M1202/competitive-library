#pragma once

#include "integer_convex_hull.hpp"
#include <algorithm>
#include <cstddef>
#include <optional>
#include <utility>
#include <vector>

namespace cp::geometry {

struct FarthestPair {
  std::size_t first, second;  // Distinct indices into the input.
  IntegerWide distance2;
};

// O(n log n) total; a strict hull and an antipodal scan. Ties are arbitrary.
inline std::optional<FarthestPair> farthest_pair(const std::vector<IntegerPoint>& points) {
  if (points.size() < 2) return std::nullopt;
  auto hull = convex_hull(points);
  if (hull.size() == 1) return FarthestPair{0, 1, 0};

  std::vector<std::pair<IntegerPoint, std::size_t>> index;
  index.reserve(points.size());
  for (std::size_t i = 0; i < points.size(); ++i) index.emplace_back(points[i], i);
  std::sort(index.begin(), index.end());
  const auto original_index = [&](IntegerPoint p) {
    auto it = std::lower_bound(index.begin(), index.end(), std::pair{p, std::size_t{0}});
    return it->second;
  };

  std::size_t best_a = 0, best_b = 1;
  IntegerWide best = squared_distance(hull[0], hull[1]);
  const auto consider = [&](std::size_t a, std::size_t b) {
    auto d = squared_distance(hull[a], hull[b]);
    if (d > best) { best = d; best_a = a; best_b = b; }
  };
  if (hull.size() > 2) {
    const std::size_t n = hull.size();
    std::size_t j = 1;
    for (std::size_t i = 0; i < n; ++i) {
      const std::size_t next = (i + 1) % n;
      while (orientation(hull[i], hull[next], hull[(j + 1) % n]) >
             orientation(hull[i], hull[next], hull[j])) j = (j + 1) % n;
      consider(i, j);
      consider(next, j);
      consider(i, (j + 1) % n);
      consider(next, (j + 1) % n);
    }
  }
  return FarthestPair{original_index(hull[best_a]), original_index(hull[best_b]), best};
}

}  // namespace cp::geometry
