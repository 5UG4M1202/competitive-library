#include <cassert>
int main() {
  using namespace cp::geometry;
  RealSegment a{ {0,0}, {2,2} }, b{ {0,2}, {2,0} };
  assert(intersects(a,b));
  auto p = line_intersection(a,b);
  assert(p && p->x == 1 && p->y == 1);
}
