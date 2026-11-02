// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/DSL_2_C
// competitive-verifier: DISPLAY no-index
#include "../../library/geometry/integer_kd_tree.hpp"
#include <algorithm>
#include <iostream>
#include <vector>
struct Count {
  using value_type = int;
  static int op(int a,int b) { return a+b; }
  static int id() { return 0; }
};
int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int n; std::cin >> n;
  std::vector<cp::geometry::IntegerPoint> points(n);
  for (auto& p : points) std::cin >> p.x >> p.y;
  cp::geometry::IntegerKdTree<Count> tree(points,std::vector<int>(n,1));
  int q; std::cin >> q;
  while (q--) {
    long long sx,tx,sy,ty; std::cin >> sx >> tx >> sy >> ty;
    std::vector<std::size_t> ids;
    tree.for_each_in_rect(sx,cp::geometry::IntegerWide(tx)+1,sy,cp::geometry::IntegerWide(ty)+1,
                          [&](std::size_t id) { ids.push_back(id); });
    std::sort(ids.begin(),ids.end());
    for (auto id : ids) std::cout << id << '\n';
    std::cout << '\n';
  }
}
