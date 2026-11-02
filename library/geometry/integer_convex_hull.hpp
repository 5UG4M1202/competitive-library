#pragma once

#include "integer_point.hpp"
#include <algorithm>
#include <cstddef>
#include <vector>

namespace cp::geometry {

// CCW boundary, without repeated starting point. Duplicates are removed.
// For a collinear set, return endpoints, or all distinct points in order.
inline std::vector<IntegerPoint> convex_hull(std::vector<IntegerPoint> points,
                                             bool keep_collinear = false) {
  std::sort(points.begin(), points.end());
  points.erase(std::unique(points.begin(), points.end()), points.end());
  if (points.size() <= 2) return points;
  bool collinear = true;
  for (std::size_t i = 2; i < points.size(); ++i) {
    if (orientation(points[0], points[1], points[i]) != 0) {
      collinear = false;
      break;
    }
  }
  if (collinear) {
    if (keep_collinear) return points;
    return {points.front(), points.back()};
  }

  std::vector<IntegerPoint> lower, upper;
  const auto append = [keep_collinear](std::vector<IntegerPoint>& chain, IntegerPoint p) {
    while (chain.size() >= 2) {
      auto turn = orientation(chain[chain.size() - 2], chain.back(), p);
      if (turn > 0 || (keep_collinear && turn == 0)) break;
      chain.pop_back();
    }
    chain.push_back(p);
  };
  for (IntegerPoint p : points) append(lower, p);
  for (auto it = points.rbegin(); it != points.rend(); ++it) append(upper, *it);
  lower.pop_back();
  upper.pop_back();
  lower.insert(lower.end(), upper.begin(), upper.end());
  return lower;
}

}  // namespace cp::geometry
