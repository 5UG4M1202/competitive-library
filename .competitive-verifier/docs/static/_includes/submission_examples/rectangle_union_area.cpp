#include <cassert>
#include <iostream>
#include <vector>
int main() {
  using namespace cp::geometry;
  std::vector<IntegerRectangle> rects;
  rects.push_back({0, 0, 3, 2}); // x=[0,3), y=[0,2)
  rects.push_back({2, 0, 4, 2});
  auto area = rectangle_union_area(rects);
  assert(area == 8);
  std::cout << static_cast<long long>(area) << '\n'; // この例は 64 ビットに収まる
}
