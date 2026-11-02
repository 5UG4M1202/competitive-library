#pragma once
#include "real_point.hpp"
#include <algorithm>
#include <vector>

namespace cp::geometry {
inline std::vector<RealPoint> convex_hull(std::vector<RealPoint> points, bool keep_collinear = false,
                                          const Geometry2D& geo = Geometry2D{}) {
  for (const auto& p : points) { assert(std::isfinite(p.x) && std::isfinite(p.y)); (void)p; }
  std::sort(points.begin(), points.end());
  points.erase(std::unique(points.begin(), points.end()), points.end());
  if (points.size() <= 2) return points;
  bool collinear = true;
  for (std::size_t i = 2; i < points.size(); ++i)
    if (geo.orientation(points[0], points[1], points[i]) != 0) { collinear = false; break; }
  if (collinear) return keep_collinear ? points : std::vector<RealPoint>{points.front(), points.back()};
  std::vector<RealPoint> lower, upper;
  auto append = [keep_collinear, &geo](std::vector<RealPoint>& chain, RealPoint p) {
    while (chain.size() >= 2) {
      int turn = geo.orientation(chain[chain.size()-2], chain.back(), p);
      if (turn > 0 || (keep_collinear && turn == 0)) break;
      chain.pop_back();
    }
    chain.push_back(p);
  };
  for (auto p : points) append(lower, p);
  for (auto it = points.rbegin(); it != points.rend(); ++it) append(upper, *it);
  lower.pop_back(); upper.pop_back();
  lower.insert(lower.end(), upper.begin(), upper.end());
  return lower;
}
}  // namespace cp::geometry
