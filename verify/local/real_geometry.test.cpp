// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/geometry/real_closest_pair.hpp"
#include "../../library/geometry/real_farthest_pair.hpp"
#include "../../library/geometry/real_convex_contains.hpp"
#include "../../library/geometry/integer_point.hpp"
#include "../../library/geometry/integer_convex_hull.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
#include <random>
#include <type_traits>
#include <vector>
using namespace cp::geometry;
int main() {
  static_assert(std::is_same_v<decltype(RealPoint{}.x), long double>);
  const double tiny = std::numeric_limits<double>::denorm_min();
  const double huge = std::numeric_limits<double>::max();
  RealPoint u{1.5, -0.5}, v{0.5, 2.0};
  assert(dot(u,v) == -0.25L && cross(u,v) == 3.25L);
  assert(norm2(u) == 2.5L && squared_distance(u,v) == 7.25L);
  assert(Geometry2D(0).orientation(RealPoint{0,0},RealPoint{tiny,0},RealPoint{0,tiny}) == 1);
  assert(orientation(RealPoint{-huge,-huge},RealPoint{huge,-huge},RealPoint{-huge,huge}) == 1);
  assert(Geometry2D(0.01L).orientation({0,0},{2,0},{1,0.005L}) == 0);
  assert(Geometry2D(0.001L).orientation({0,0},{2,0},{1,0.005L}) == 1);
  Point<int,3> vec_a{{1,2,3}}, vec_b{{4,5,6}};
  assert(dot_n<long long>(vec_a,vec_b) == 32);
  assert(squared_distance_n<long long>(vec_a,vec_b) == 27);
  auto cp = cross3<long long>(vec_a,vec_b);
  assert(cp[0] == -3 && cp[1] == 6 && cp[2] == -3);
  Point<long long,2> p2{3,4};
  assert(p2[0] == 3 && p2[1] == 4);
  p2[1] = 5;
  assert(p2.y == 5 && cross2<long long>(p2, Point<long long,2>{1,1}) == -2);
  Point<int,1> p1{{7}};
  assert(p1[0] == 7 && dot_n<long long>(p1,p1) == 49);
  assert(same_direction(RealPoint{tiny,tiny}, RealPoint{huge,huge}));
  assert(opposite_direction(RealPoint{tiny,tiny}, RealPoint{-huge,-huge}));
  assert(!same_direction(RealPoint{0,0}, RealPoint{1,0}));
  std::vector<RealPoint> angles = {{0,-1},{1,0},{1,1},{-1,0},{-1,-1},{0,0},
                                   {tiny,tiny},{huge,huge},{1,std::nextafter(1.0,2.0)}};
  const RealArgumentLess less;
  for (const auto& a : angles) for (const auto& b : angles) for (const auto& c : angles) {
    if (less(a,b) && less(b,c)) assert(less(a,c));
    if (!less(a,b) && !less(b,a) && !less(b,c) && !less(c,b))
      assert(!less(a,c) && !less(c,a));
  }
  std::sort(angles.begin(), angles.end(), RealArgumentLess{});
  for (std::size_t i=1;i<angles.size();++i) assert(!RealArgumentLess{}(angles[i],angles[i-1]));
  std::vector<RealPoint> square = {{0,0},{2,0},{2,2},{0,2},{1,1},{0,0}};
  auto hull = convex_hull(square);
  assert(hull.size() == 4);
  assert(convex_contains(hull,{1,1}) == RealPointLocation::inside);
  assert(convex_contains(hull,{2,1}) == RealPointLocation::boundary);
  assert(convex_contains(hull,{3,1}) == RealPointLocation::outside);
  assert(convex_contains(hull,{1,2.005L},Geometry2D(0.01L)) == RealPointLocation::boundary);
  assert(convex_contains(hull,{1,2.005L},Geometry2D(0.001L)) == RealPointLocation::outside);
  assert(convex_contains(std::vector<RealPoint>{},{0,0}) == RealPointLocation::outside);
  assert(convex_contains(std::vector<RealPoint>{{1,1}},{1,1}) == RealPointLocation::boundary);
  assert(convex_contains(std::vector<RealPoint>{{0,0},{2,2}},{1,1}) == RealPointLocation::boundary);
  assert(convex_contains(std::vector<RealPoint>{{0,0},{2,2}},{1,2}) == RealPointLocation::outside);
  std::vector<RealPoint> line = {{0,0},{1,1},{2,2},{1,1}};
  assert(convex_hull(line).size() == 2);
  assert(convex_hull(line,true).size() == 3);
  std::vector<RealPoint> almost_line = {{0,0},{1,0.005L},{2,0}};
  assert(convex_hull(almost_line,false,Geometry2D(0.01L)).size() == 2);
  assert(convex_hull(almost_line,false,Geometry2D(0.001L)).size() == 3);
  assert(closest_pair(square)->distance2 == 0);
  assert(farthest_pair(square)->distance2 == 8);
  assert(!closest_pair(std::vector<RealPoint>{{0,0}}));
  assert(!farthest_pair(std::vector<RealPoint>{}));
  auto tiny_pair = closest_pair(std::vector<RealPoint>{{0,0},{tiny,0}});
  assert(tiny_pair && tiny_pair->distance2 == std::ldexp(1.0L,-2148));
  auto big_pair = farthest_pair(std::vector<RealPoint>{{-huge,0},{huge,0}});
  assert(big_pair && big_pair->distance2 > 0);
  std::mt19937 rng(309);
  for (int t=0;t<250;++t) {
    std::vector<RealPoint> p;
    std::vector<IntegerPoint> pi;
    const int n=2+static_cast<int>(rng()%25);
    for (int i=0;i<n;++i) {
      const int x=static_cast<int>(rng()%81)-40, y=static_cast<int>(rng()%81)-40;
      p.push_back({x/8.0,y/8.0});
      pi.push_back({x,y});
    }
    auto rh = convex_hull(p);
    auto ih = convex_hull(pi);
    assert(rh.size() == ih.size());
    for (std::size_t i=0;i<rh.size();++i)
      assert(rh[i].x*8==static_cast<double>(ih[i].x) && rh[i].y*8==static_cast<double>(ih[i].y));
    auto near = closest_pair(p);
    auto far = farthest_pair(p);
    long long min_int = std::numeric_limits<long long>::max(), max_int = 0;
    for (int i=0;i<n;++i) for (int j=0;j<i;++j) {
      long long dx = pi[i].x - pi[j].x, dy = pi[i].y - pi[j].y;
      const auto distance_int = dx*dx + dy*dy;
      min_int = std::min(min_int, distance_int);
      max_int = std::max(max_int, distance_int);
    }
    assert(squared_distance(p[near->first],p[near->second]) == static_cast<long double>(min_int)/64);
    assert(squared_distance(p[far->first],p[far->second]) == static_cast<long double>(max_int)/64);
    assert(near->distance2 == static_cast<long double>(min_int)/64);
    assert(far->distance2 == static_cast<long double>(max_int)/64);
  }
}
