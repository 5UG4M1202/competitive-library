// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/furthest_pair
// competitive-verifier: DISPLAY no-index
#include "../../library/geometry/integer_farthest_pair.hpp"
#include <iostream>
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
    auto result = cp::geometry::farthest_pair(points);
    std::cout << result->first << ' ' << result->second << '\n';
  }
}
