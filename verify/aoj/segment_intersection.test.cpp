// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/CGL_2_B
// competitive-verifier: DISPLAY no-index
#include "../../library/geometry/integer_segment.hpp"
#include <iostream>
int main() {
  using namespace cp::geometry;
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int q; std::cin >> q;
  while (q--) {
    IntegerSegment a,b;
    std::cin >> a.a.x >> a.a.y >> a.b.x >> a.b.y >> b.a.x >> b.a.y >> b.b.x >> b.b.y;
    std::cout << (intersection_kind(a,b) != IntegerSegmentIntersection::none) << '\n';
  }
}
