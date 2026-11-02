// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/rectangle_sum
// competitive-verifier: DISPLAY no-index
#include "../../library/geometry/integer_kd_tree.hpp"
#include <iostream>
#include <vector>

struct Sum {
  using value_type = long long;
  static value_type id() { return 0; }
  static value_type op(value_type a, value_type b) { return a + b; }
};
int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int n, q;
  std::cin >> n >> q;
  std::vector<cp::geometry::IntegerPoint> points(n);
  std::vector<long long> values(n);
  for (int i = 0; i < n; ++i) std::cin >> points[i].x >> points[i].y >> values[i];
  cp::geometry::IntegerKdTree<Sum> tree(points, values);
  while (q--) {
    long long xl, yl, xr, yr;
    std::cin >> xl >> yl >> xr >> yr;
    std::cout << tree.prod(xl, xr, yl, yr) << '\n';
  }
}
