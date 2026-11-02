// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/static_convex_hull
// competitive-verifier: DISPLAY no-index
#include "../../library/geometry/integer_convex_hull.hpp"
#include <iostream>
#include <utility>
#include <vector>

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int t;
  std::cin >> t;
  while (t--) {
    int n;
    std::cin >> n;
    std::vector<cp::geometry::IntegerPoint> points(n);
    for (auto& p : points) std::cin >> p.x >> p.y;
    auto hull = cp::geometry::convex_hull(std::move(points));
    std::cout << hull.size() << '\n';
    for (auto p : hull) std::cout << p.x << ' ' << p.y << '\n';
  }
}
