// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/geometry/rectangle_union_area.hpp"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>
#include <random>
#include <stdexcept>
#include <vector>
using cp::geometry::IntegerRectangle;
using cp::geometry::rectangle_union_area;
int main() {
  assert(rectangle_union_area({})==0);
  assert(rectangle_union_area({{0,0,3,2},{2,0,4,2}})==8);
  assert(rectangle_union_area({{0,0,2,2},{0,0,2,2},{2,0,4,2},{1,1,1,5}})==8);
  const auto lo=std::numeric_limits<std::int64_t>::min(), hi=std::numeric_limits<std::int64_t>::max();
  assert(rectangle_union_area({{lo,0,hi,1}})==__int128_t(hi)-lo);
  bool overflow=false;
  try { (void)rectangle_union_area({{lo,lo,hi,hi}}); } catch (const std::overflow_error&) { overflow=true; }
  assert(overflow);
  overflow=false;
  try { (void)rectangle_union_area({{lo,lo,0,hi},{0,lo,hi,hi}}); } catch (const std::overflow_error&) { overflow=true; }
  assert(overflow); // Both strips fit; their sum does not.
  std::mt19937 rng(53124);
  for (int trial=0;trial<5000;++trial) {
    std::vector<IntegerRectangle> rectangles;
    const auto n=rng()%12;
    for (unsigned i=0;i<n;++i) {
      auto x1=static_cast<int>(rng()%11)-5, x2=static_cast<int>(rng()%11)-5;
      auto y1=static_cast<int>(rng()%11)-5, y2=static_cast<int>(rng()%11)-5;
      if (x1>x2) std::swap(x1,x2);
      if (y1>y2) std::swap(y1,y2);
      rectangles.push_back({x1,y1,x2,y2});
    }
    long long expected=0;
    for (int x=-5;x<5;++x) for (int y=-5;y<5;++y) {
      bool covered=false;
      for (const auto& r:rectangles) covered |= r.xl<=x && x<r.xr && r.yl<=y && y<r.yr;
      expected+=covered;
    }
    assert(rectangle_union_area(rectangles)==expected);
  }
}
