// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/vertex_set_path_composite
// competitive-verifier: DISPLAY no-index
// Tests noncommutative vertex-path composition and point replacement.
#include "../../library/graph/heavy_light_decomposition.hpp"
#include "../../library/data_structure/monoid_segtree.hpp"
#include <atcoder/modint>
#include <iostream>
#include <vector>

using Mint = atcoder::modint998244353;
struct Affine {
  struct value_type { Mint a, b; };
  // Traverse left first, then right: right(left(x)).
  static value_type op(value_type left, value_type right) {
    return {right.a * left.a, right.a * left.b + right.b};
  }
  static value_type id() { return {1, 0}; }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int n, q;
  std::cin >> n >> q;
  std::vector<Affine::value_type> initial(n);
  for (auto& x : initial) {
    int a, b;
    std::cin >> a >> b;
    x = {a, b};
  }
  std::vector<std::vector<int>> graph(n);
  for (int i = 1; i < n; ++i) {
    int u, v;
    std::cin >> u >> v;
    graph[u].push_back(v);
    graph[v].push_back(u);
  }
  cp::HeavyLightDecomposition hld(graph);
  using B = cp::BidirectionalMonoid<Affine>;
  std::vector<B::value_type> arranged(n, B::id());
  for (int v = 0; v < n; ++v) arranged[hld.index(v)] = B::vertex(initial[v]);
  cp::Segtree<B> seg(arranged);
  auto fold = cp::make_hld_path_fold<Affine>(hld, seg);
  while (q--) {
    int type;
    std::cin >> type;
    if (type == 0) {
      int p, a, b;
      std::cin >> p >> a >> b;
      fold.set_vertex(p, {a, b});
    } else {
      int u, v, x;
      std::cin >> u >> v >> x;
      auto f = fold.vertices(u, v);
      std::cout << (f.a * Mint(x) + f.b).val() << '\n';
    }
  }
}
