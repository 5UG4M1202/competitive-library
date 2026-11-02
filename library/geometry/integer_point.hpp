#pragma once

#include "point.hpp"
#include <cstdint>

namespace cp::geometry {

// Integer coordinates; arithmetic operations require their results to fit.
using IntegerPoint = Point<std::int64_t, 2>;

using IntegerWide = __int128_t;  // GCC/Clang extension.

inline IntegerWide dot(IntegerPoint a, IntegerPoint b) {
  return IntegerWide(a.x) * b.x + IntegerWide(a.y) * b.y;
}
inline IntegerWide cross(IntegerPoint a, IntegerPoint b) {
  return IntegerWide(a.x) * b.y - IntegerWide(a.y) * b.x;
}
inline IntegerWide norm2(IntegerPoint a) { return dot(a, a); }
// Widen before subtracting; every intermediate must fit in IntegerWide.
inline IntegerWide squared_distance(IntegerPoint a, IntegerPoint b) {
  const IntegerWide dx = IntegerWide(a.x) - b.x;
  const IntegerWide dy = IntegerWide(a.y) - b.y;
  return dx * dx + dy * dy;
}

// Positive: c is left of a->b; negative: right; zero: collinear.
// Widen before subtraction as well as multiplication.
inline IntegerWide orientation(IntegerPoint a, IntegerPoint b, IntegerPoint c) {
  const IntegerWide bx = IntegerWide(b.x) - a.x, by = IntegerWide(b.y) - a.y;
  const IntegerWide cx = IntegerWide(c.x) - a.x, cy = IntegerWide(c.y) - a.y;
  return bx * cy - by * cx;
}

// For nonzero vectors, distinguish parallel directions from opposite ones.
inline bool same_direction(IntegerPoint a, IntegerPoint b) {
  return (a.x != 0 || a.y != 0) && (b.x != 0 || b.y != 0) &&
         cross(a, b) == 0 && dot(a, b) > 0;
}
inline bool opposite_direction(IntegerPoint a, IntegerPoint b) {
  return (a.x != 0 || a.y != 0) && (b.x != 0 || b.y != 0) &&
         cross(a, b) == 0 && dot(a, b) < 0;
}

// CCW order in (-pi, pi]. The zero vector is ordered at angle 0, and
// vectors with the same angle compare equivalent regardless of length.
struct ArgumentLess {
  bool operator()(IntegerPoint a, IntegerPoint b) const {
    auto group = [](IntegerPoint p) {
      if (p.y < 0) return 0;
      if (p.y == 0 && p.x >= 0) return 1;
      return 2;
    };
    const int ga = group(a), gb = group(b);
    if (ga != gb) return ga < gb;
    return cross(a, b) > 0;
  }
};

}  // namespace cp::geometry
