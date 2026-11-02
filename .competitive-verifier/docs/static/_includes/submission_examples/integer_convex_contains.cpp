#include <cassert>
#include <vector>
using namespace cp::geometry;

int main() {
  auto hull = convex_hull(std::vector<IntegerPoint>{ {0, 0}, {4, 0}, {4, 4}, {0, 4} });
  assert(convex_contains(hull, {2, 2}) == PointLocation::inside);
  assert(convex_contains(hull, {4, 2}) == PointLocation::boundary);
  assert(convex_contains(hull, {5, 2}) == PointLocation::outside);
}
