#pragma once

#include "integer_point.hpp"
#include <algorithm>
#include <cstddef>
#include <vector>

namespace cp::geometry {

enum class PointLocation { outside, boundary, inside };

// The endpoints are included. All orientation intermediates must fit.
inline bool on_segment(IntegerPoint a, IntegerPoint b, IntegerPoint p) {
  return orientation(a, b, p) == 0 &&
      std::min(a.x, b.x) <= p.x && p.x <= std::max(a.x, b.x) &&
      std::min(a.y, b.y) <= p.y && p.y <= std::max(a.y, b.y);
}

// For n >= 3, polygon must be strictly convex, CCW, and not closed by
// repeating the first vertex. Empty, one-point and segment hulls are allowed.
inline PointLocation convex_contains(const std::vector<IntegerPoint>& polygon, IntegerPoint p) {
  const std::size_t n = polygon.size();
  if (n == 0) return PointLocation::outside;
  if (n == 1) return p == polygon[0] ? PointLocation::boundary : PointLocation::outside;
  if (n == 2) return on_segment(polygon[0], polygon[1], p)
                         ? PointLocation::boundary : PointLocation::outside;
  auto left = orientation(polygon[0], polygon[1], p);
  auto right = orientation(polygon[0], polygon[n - 1], p);
  if (left < 0 || right > 0) return PointLocation::outside;
  if (left == 0) return on_segment(polygon[0], polygon[1], p)
                             ? PointLocation::boundary : PointLocation::outside;
  if (right == 0) return on_segment(polygon[0], polygon[n - 1], p)
                              ? PointLocation::boundary : PointLocation::outside;
  std::size_t lo = 1, hi = n - 1;
  while (hi - lo > 1) {
    const std::size_t mid = lo + (hi - lo) / 2;
    if (orientation(polygon[0], polygon[mid], p) >= 0) lo = mid;
    else hi = mid;
  }
  auto side = orientation(polygon[lo], polygon[hi], p);
  if (side < 0) return PointLocation::outside;
  if (side == 0) return PointLocation::boundary;
  return PointLocation::inside;
}

}  // namespace cp::geometry
