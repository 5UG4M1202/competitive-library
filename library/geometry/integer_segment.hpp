#pragma once

#include "integer_point.hpp"
#include "../math/rational128.hpp"
#include <algorithm>
#include <optional>
#include <utility>

namespace cp::geometry {

struct IntegerSegment { IntegerPoint a, b; };
enum class IntegerSegmentIntersection { none, point, overlap };

inline bool contains(IntegerSegment s, IntegerPoint p) {
  return orientation(s.a,s.b,p) == 0 &&
         std::min(s.a.x,s.b.x) <= p.x && p.x <= std::max(s.a.x,s.b.x) &&
         std::min(s.a.y,s.b.y) <= p.y && p.y <= std::max(s.a.y,s.b.y);
}

inline IntegerSegmentIntersection intersection_kind(IntegerSegment s, IntegerSegment t) {
  const auto a = orientation(s.a,s.b,t.a), b = orientation(s.a,s.b,t.b);
  const auto c = orientation(t.a,t.b,s.a), d = orientation(t.a,t.b,s.b);
  if (a == 0 && b == 0 && c == 0 && d == 0) {
    if (s.b < s.a) std::swap(s.a,s.b);
    if (t.b < t.a) std::swap(t.a,t.b);
    const auto lo = std::max(s.a,t.a), hi = std::min(s.b,t.b);
    return hi < lo ? IntegerSegmentIntersection::none : lo == hi ? IntegerSegmentIntersection::point : IntegerSegmentIntersection::overlap;
  }
  if ((a == 0 && contains(s,t.a)) || (b == 0 && contains(s,t.b)) ||
      (c == 0 && contains(t,s.a)) || (d == 0 && contains(t,s.b))) return IntegerSegmentIntersection::point;
  return ((a < 0 && b > 0) || (a > 0 && b < 0)) && ((c < 0 && d > 0) || (c > 0 && d < 0))
      ? IntegerSegmentIntersection::point : IntegerSegmentIntersection::none;
}

// Infinite lines through the two endpoints. Degenerate/parallel/coincident: nullopt.
// All integer intermediate products/subtractions must fit in IntegerWide.
inline std::optional<Point<cp::Rational128,2>> line_intersection(IntegerSegment s, IntegerSegment t) {
  const IntegerWide sx = IntegerWide(s.b.x)-s.a.x, sy = IntegerWide(s.b.y)-s.a.y;
  const IntegerWide tx = IntegerWide(t.b.x)-t.a.x, ty = IntegerWide(t.b.y)-t.a.y;
  const IntegerWide dx = IntegerWide(t.a.x)-s.a.x, dy = IntegerWide(t.a.y)-s.a.y;
  const IntegerWide den = sx*ty-sy*tx;
  if (den == 0) return std::nullopt;
  const IntegerWide num = dx*ty-dy*tx;
  const cp::Rational128 ratio(num,den);
  return Point<cp::Rational128,2>{cp::Rational128(s.a.x)+cp::Rational128(sx)*ratio,
                                   cp::Rational128(s.a.y)+cp::Rational128(sy)*ratio};
}
}  // namespace cp::geometry
