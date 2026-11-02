#pragma once
#include "real_point_base.hpp"
#include <cassert>
#include <cmath>
#include <numbers>

namespace cp::geometry {
inline long double dot(RealPoint a, RealPoint b) { return a.x*b.x + a.y*b.y; }
inline long double cross(RealPoint a, RealPoint b) { return a.x*b.y - a.y*b.x; }
inline long double norm2(RealPoint a) { return dot(a,a); }
inline long double squared_distance(RealPoint a, RealPoint b) { return norm2(a-b); }

// eps is an absolute distance in coordinate units, not an area threshold.
class Geometry2D {
 public:
  explicit Geometry2D(long double eps = 1e-10L) : eps_(eps) { assert(std::isfinite(eps) && eps >= 0); }
  long double epsilon() const { return eps_; }
  int orientation(RealPoint a, RealPoint b, RealPoint c) const {
    const auto v = b-a, w = c-a;
    const auto len = std::hypot(v.x,v.y);
    if (len == 0) return 0;
    // Dividing before multiplying avoids overflow of eps * length.
    const long double signed_distance = (v.x/len)*w.y - (v.y/len)*w.x;
    return signed_distance > eps_ ? 1 : signed_distance < -eps_ ? -1 : 0;
  }
 private:
  long double eps_;
};
inline int orientation(RealPoint a, RealPoint b, RealPoint c) { return Geometry2D{}.orientation(a,b,c); }
inline bool same_direction(RealPoint a, RealPoint b, const Geometry2D& geo = Geometry2D{}) {
  if ((a.x == 0 && a.y == 0) || (b.x == 0 && b.y == 0)) return false;
  return geo.orientation({0,0},a,b) == 0 && dot(a,b) > 0;
}
inline bool opposite_direction(RealPoint a, RealPoint b, const Geometry2D& geo = Geometry2D{}) {
  if ((a.x == 0 && a.y == 0) || (b.x == 0 && b.y == 0)) return false;
  return geo.orientation({0,0},a,b) == 0 && dot(a,b) < 0;
}
// Ordering uses computed angle keys, never EPS. Finite coordinates required.
struct RealArgumentLess {
  static long double key(RealPoint p) {
    if (p.y == 0 && p.x < 0) return std::numbers::pi_v<long double>;
    if (p.x == 0 && p.y == 0) return 0;
    return std::atan2(p.y,p.x);
  }
  bool operator()(RealPoint a, RealPoint b) const { return key(a) < key(b); }
};
}
