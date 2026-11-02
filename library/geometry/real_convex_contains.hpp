#pragma once
#include "real_point.hpp"
#include <algorithm>
#include <vector>

namespace cp::geometry {
enum class RealPointLocation { outside, boundary, inside };
inline bool on_segment(RealPoint a, RealPoint b, RealPoint p, const Geometry2D& geo = Geometry2D{}) {
  if (a == b) return std::hypot(p.x-a.x,p.y-a.y) <= geo.epsilon();
  const auto e = geo.epsilon();
  return geo.orientation(a, b, p) == 0 &&
         std::min(a.x,b.x)-e <= p.x && p.x <= std::max(a.x,b.x)+e &&
         std::min(a.y,b.y)-e <= p.y && p.y <= std::max(a.y,b.y)+e;
}
inline RealPointLocation convex_contains(const std::vector<RealPoint>& polygon, RealPoint p,
                                          const Geometry2D& geo = Geometry2D{}) {
  const std::size_t n = polygon.size();
  if (!n) return RealPointLocation::outside;
  if (n == 1) return on_segment(polygon[0],polygon[0],p,geo) ? RealPointLocation::boundary : RealPointLocation::outside;
  if (n == 2) return on_segment(polygon[0],polygon[1],p,geo) ? RealPointLocation::boundary : RealPointLocation::outside;
  const auto left = geo.orientation(polygon[0],polygon[1],p), right = geo.orientation(polygon[0],polygon[n-1],p);
  if (left < 0 || right > 0) return RealPointLocation::outside;
  if (left == 0) return on_segment(polygon[0],polygon[1],p,geo) ? RealPointLocation::boundary : RealPointLocation::outside;
  if (right == 0) return on_segment(polygon[0],polygon[n-1],p,geo) ? RealPointLocation::boundary : RealPointLocation::outside;
  std::size_t lo = 1, hi = n - 1;
  while (hi - lo > 1) {
    const auto mid = lo + (hi - lo) / 2;
    if (geo.orientation(polygon[0],polygon[mid],p) >= 0) lo = mid; else hi = mid;
  }
  const auto side = geo.orientation(polygon[lo],polygon[hi],p);
  return side < 0 ? RealPointLocation::outside : side == 0 ? RealPointLocation::boundary : RealPointLocation::inside;
}
}
