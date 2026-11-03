#include <cassert>
int main() {
  using namespace cp::geometry;
  IntegerSegment a{ {0,0}, {2,2} }, b{ {0,2}, {2,0} };
  assert(intersection_kind(a,b) == IntegerSegmentIntersection::point);
  auto p = line_intersection(a,b);
  assert(p && p->x == cp::Rational128(1) && p->y == cp::Rational128(1));
}
