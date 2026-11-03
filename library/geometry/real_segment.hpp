#pragma once

#include "real_point.hpp"
#include <algorithm>
#include <cmath>
#include <optional>

namespace cp::geometry {

struct RealSegment { RealPoint a, b; };

// EPS is an absolute distance; endpoints and intermediate results must be finite.
inline bool contains(RealSegment s, RealPoint p, const Geometry2D& geo = Geometry2D{}) {
  if (p == s.a || p == s.b) return true;
  const auto v = s.b-s.a;
  const auto length = std::hypot(v.x,v.y);
  if (length == 0) return std::hypot(p.x-s.a.x,p.y-s.a.y) <= geo.epsilon();
  const auto x = p.x-s.a.x, y = p.y-s.a.y;
  const auto along = x*(v.x/length)+y*(v.y/length);
  const auto perpendicular = x*(v.y/length)-y*(v.x/length);
  if (along < 0) return std::hypot(along,perpendicular) <= geo.epsilon();
  if (along > length) return std::hypot(along-length,perpendicular) <= geo.epsilon();
  return std::abs(perpendicular) <= geo.epsilon();
}

inline bool intersects(RealSegment s, RealSegment t, const Geometry2D& geo = Geometry2D{}) {
  const int a = geo.orientation(s.a,s.b,t.a), b = geo.orientation(s.a,s.b,t.b);
  const int c = geo.orientation(t.a,t.b,s.a), d = geo.orientation(t.a,t.b,s.b);
  if ((a == 0 && contains(s,t.a,geo)) || (b == 0 && contains(s,t.b,geo)) ||
      (c == 0 && contains(t,s.a,geo)) || (d == 0 && contains(t,s.b,geo))) return true;
  return a*b < 0 && c*d < 0;
}

// Infinite lines. Parallel, coincident and degenerate lines yield nullopt.
inline std::optional<RealPoint> line_intersection(RealSegment s, RealSegment t, const Geometry2D& geo = Geometry2D{}) {
  const auto v=s.b-s.a, w=t.b-t.a;
  const auto lv=std::hypot(v.x,v.y), lw=std::hypot(w.x,w.y);
  if (lv == 0 || lw == 0) return std::nullopt;
  const auto den = cross(v,w);
  if (std::abs(den/lv) <= geo.epsilon() || std::abs(den/lw) <= geo.epsilon()) return std::nullopt;
  const auto ratio = cross(t.a-s.a,w)/den;
  return RealPoint{s.a.x+v.x*ratio, s.a.y+v.y*ratio};
}

inline RealPoint projection(RealSegment line, RealPoint p) {
  const auto v=line.b-line.a;
  const auto length2=norm2(v);
  assert(length2 > 0);
  const auto ratio=dot(p-line.a,v)/length2;
  return {line.a.x+ratio*v.x,line.a.y+ratio*v.y};
}
inline long double segment_distance(RealSegment s, RealPoint p) {
  const auto v=s.b-s.a;
  const auto length2=norm2(v);
  if (length2 == 0) return std::hypot(p.x-s.a.x,p.y-s.a.y);
  const auto ratio=std::clamp(dot(p-s.a,v)/length2,0.0L,1.0L);
  return std::hypot(p.x-(s.a.x+ratio*v.x),p.y-(s.a.y+ratio*v.y));
}
}  // namespace cp::geometry
