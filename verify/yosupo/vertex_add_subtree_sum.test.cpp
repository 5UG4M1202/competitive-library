// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/vertex_add_subtree_sum
// competitive-verifier: DISPLAY no-index
// Checks HLD subtree intervals, point additions and range sums.
#include "../../library/graph/heavy_light_decomposition.hpp"
#include "../../library/data_structure/monoid_segtree.hpp"
#include <iostream>
#include <vector>

struct Sum {
  using value_type = long long;
  static value_type op(value_type a, value_type b) { return a + b; }
  static value_type id() { return 0; }
};
int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int n, q;
  std::cin >> n >> q;
  std::vector<long long> a(n);
  for (auto& x : a) std::cin >> x;
  std::vector<std::vector<int>> graph(n);
  for (int v = 1; v < n; ++v) {
    int p;
    std::cin >> p;
    graph[p].push_back(v);
    graph[v].push_back(p);
  }
  cp::HeavyLightDecomposition hld(graph);
  std::vector<long long> arranged(n);
  for (int v = 0; v < n; ++v) arranged[hld.index(v)] = a[v];
  cp::Segtree<Sum> seg(arranged);
  while (q--) {
    int t, v;
    std::cin >> t >> v;
    if (t == 0) {
      long long x;
      std::cin >> x;
      seg.set(hld.index(v), seg.get(hld.index(v)) + x);
    } else {
      auto [l, r] = hld.subtree_vertices(v);
      std::cout << seg.prod(l, r) << '\n';
    }
  }
}
