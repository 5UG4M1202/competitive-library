// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
// Intended correctness: path and subtree updates compared with a naive model.
#include "../../library/graph/heavy_light_decomposition.hpp"
#include "../../library/data_structure/monoid_segtree.hpp"

#include <algorithm>
#include <cassert>
#include <queue>
#include <random>
#include <vector>

int main() {
  std::mt19937 rng(309112);
  for (int trial = 0; trial < 40; ++trial) {
    const int n = 1 + static_cast<int>(rng() % 25);
    std::vector<std::vector<int>> graph(n);
    for (int v = 1; v < n; ++v) {
      int p = static_cast<int>(rng() % static_cast<unsigned>(v));
      graph[v].push_back(p); graph[p].push_back(v);
    }
    int root = static_cast<int>(rng() % static_cast<unsigned>(n));
    cp::HeavyLightDecomposition hld(graph, root);
    std::vector<int> parent(n, -2);
    parent[root] = -1;
    std::queue<int> q;
    q.push(root);
    while (!q.empty()) {
      int v = q.front(); q.pop();
      for (int to : graph[v]) if (parent[to] == -2) { parent[to] = v; q.push(to); }
    }
    using P = cp::RangeAdd<long long, cp::AggregateKind::sum>;
    cp::LazySegtree<P> seg(cp::range_values<P>(std::vector<long long>(n, 0)));
    std::vector<long long> model(n, 0);
    for (int iter = 0; iter < 500; ++iter) {
      int u = static_cast<int>(rng() % static_cast<unsigned>(n));
      int v = static_cast<int>(rng() % static_cast<unsigned>(n));
      long long add = static_cast<long long>(rng() % 11) - 5;
      if (rng() & 1U) {
        std::vector<int> prev(n, -1);
        std::queue<int> paths;
        prev[u] = u; paths.push(u);
        while (!paths.empty()) {
          int x = paths.front(); paths.pop();
          for (int to : graph[x]) if (prev[to] == -1) { prev[to] = x; paths.push(to); }
        }
        for (int x = v;; x = prev[x]) { model[x] += add; if (x == u) break; }
        for (auto s : hld.path_vertices(u, v)) seg.apply(s.l, s.r, add);
      } else {
        for (int x = 0; x < n; ++x) {
          int cur = x;
          while (cur != -1 && cur != v) cur = parent[cur];
          if (cur == v) model[x] += add;
        }
        auto [l, r] = hld.subtree_vertices(v);
        seg.apply(l, r, add);
      }
      for (int x = 0; x < n; ++x) assert(seg.get(hld.index(x)).value == model[x]);
      auto [l, r] = hld.subtree_vertices(v);
      long long expected = 0;
      for (int x = 0; x < n; ++x) {
        int cur = x;
        while (cur != -1 && cur != v) cur = parent[cur];
        if (cur == v) expected += model[x];
      }
      assert(seg.prod(l, r).value == expected);
    }
  }
}
