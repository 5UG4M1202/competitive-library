// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/sort_points_by_argument
// competitive-verifier: DISPLAY no-index
#include "../../library/geometry/integer_point.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int n;
  std::cin >> n;
  std::vector<cp::geometry::IntegerPoint> points(n);
  for (auto& p : points) std::cin >> p.x >> p.y;
  std::sort(points.begin(), points.end(), cp::geometry::ArgumentLess{});
  for (auto p : points) std::cout << p.x << ' ' << p.y << '\n';
}
