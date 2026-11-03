// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/area_of_union_of_rectangles
// competitive-verifier: DISPLAY no-index
#include "../../library/geometry/rectangle_union_area.hpp"
#include <iostream>
#include <vector>
int main() {
  using namespace cp::geometry;
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::size_t n; std::cin >> n;
  std::vector<IntegerRectangle> rectangles(n);
  for (auto& r:rectangles) std::cin >> r.xl >> r.yl >> r.xr >> r.yr;
  // This problem's coordinates ensure the area fits signed 64 bits.
  std::cout << static_cast<long long>(rectangle_union_area(rectangles)) << '\n';
}
