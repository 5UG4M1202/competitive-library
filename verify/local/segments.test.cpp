// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/geometry/integer_segment.hpp"
#include "../../library/geometry/real_segment.hpp"
#include <cassert>
#include <cmath>
#include <vector>
using namespace cp::geometry;
int main() {
  using K=IntegerSegmentIntersection;
  assert(intersection_kind({{0,0},{4,0}},{{2,0},{6,0}})==K::overlap);
  assert(intersection_kind({{0,0},{4,0}},{{4,0},{6,0}})==K::point);
  assert(intersection_kind({{0,0},{4,0}},{{5,0},{6,0}})==K::none);
  assert(intersection_kind({{1,1},{1,1}},{{0,0},{2,2}})==K::point);
  assert(intersection_kind({{1,0},{1,0}},{{0,0},{2,2}})==K::none);
  assert(intersection_kind({{1,1},{1,1}},{{1,1},{1,1}})==K::point);
  assert(intersection_kind({{1,1},{1,1}},{{2,2},{2,2}})==K::none);
  auto p=line_intersection(IntegerSegment{{0,0},{1,1}},IntegerSegment{{0,1},{1,0}});
  assert(p && p->x==cp::Rational128(1,2) && p->y==cp::Rational128(1,2));
  assert(!line_intersection(IntegerSegment{{0,0},{1,0}},IntegerSegment{{0,2},{1,2}}));
  assert(!line_intersection(IntegerSegment{{0,0},{0,0}},IntegerSegment{{0,0},{1,0}}));
  const Geometry2D exact(0);
  std::vector<IntegerPoint> points;
  for (int x=-1;x<=1;++x) for (int y=-1;y<=1;++y) points.push_back({x,y});
  for (auto a:points) for (auto b:points) for (auto c:points) for (auto d:points) {
    RealSegment s{{static_cast<long double>(a.x),static_cast<long double>(a.y)}, {static_cast<long double>(b.x),static_cast<long double>(b.y)}};
    RealSegment t{{static_cast<long double>(c.x),static_cast<long double>(c.y)}, {static_cast<long double>(d.x),static_cast<long double>(d.y)}};
    assert(intersects(s,t,exact)==(intersection_kind({a,b},{c,d})!=K::none));
    assert(intersection_kind({a,b},{c,d})==intersection_kind({d,c},{b,a}));
  }
  const Geometry2D eps(1e-8L);
  assert(contains(RealSegment{{0,0},{1,0}},RealPoint{0.5L,1e-9L},eps));
  assert(!contains(RealSegment{{0,0},{1,0}},RealPoint{0.5L,1e-6L},eps));
  assert(contains(RealSegment{{0,0},{1,0}},RealPoint{-0.6e-8L,0.6e-8L},eps));
  assert(!contains(RealSegment{{0,0},{1,0}},RealPoint{-0.8e-8L,0.8e-8L},eps));
  auto q=line_intersection(RealSegment{{0,0},{1,1}},RealSegment{{0,1},{1,0}},exact);
  assert(q && std::abs(q->x-0.5L)<1e-15L && std::abs(q->y-0.5L)<1e-15L);
  RealSegment near_a{{0,0},{100,0}}, near_b{{0,1},{1,1+1e-10L}};
  assert(!line_intersection(near_a,near_b,eps) && !line_intersection(near_b,near_a,eps));
  auto foot=projection({{0,0},{4,0}},{2,3});
  assert(foot.x==2 && foot.y==0);
  assert(segment_distance({{0,0},{2,0}},{3,1})==std::sqrt(2.0L));
}
