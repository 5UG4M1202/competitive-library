// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/point_add_rectangle_sum
// competitive-verifier: DISPLAY no-index
#include "../../library/data_structure/sparse_segtree_2d.hpp"
#include <iostream>
#include <utility>
#include <vector>

struct Sum {
  using value_type = long long;
  static value_type id() { return 0; }
  static value_type op(value_type a, value_type b) { return a + b; }
};
struct Query { int type; long long x, y, z, w; };
int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int n, q;
  std::cin >> n >> q;
  std::vector<Query> initial(static_cast<std::size_t>(n));
  std::vector<Query> queries(static_cast<std::size_t>(q));
  std::vector<std::pair<long long,long long>> points;
  for (auto& p : initial) {
    std::cin >> p.x >> p.y >> p.z;
    points.emplace_back(p.x, p.y);
  }
  for (auto& p : queries) {
    std::cin >> p.type >> p.x >> p.y >> p.z;
    if (p.type == 0) points.emplace_back(p.x, p.y);
    else std::cin >> p.w;
  }
  cp::SparseSegtree2D<long long,Sum> tree(points);
  for (const auto& p : initial) tree.set(p.x,p.y,tree.get(p.x,p.y)+p.z);
  for (const auto& p : queries) {
    if (p.type == 0) tree.set(p.x,p.y,tree.get(p.x,p.y)+p.z);
    else std::cout << tree.prod(p.x,p.z,p.y,p.w) << '\n';
  }
}
