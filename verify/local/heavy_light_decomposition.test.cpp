// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
// Intended correctness: all directed paths and subtrees against a naive tree.
#include "../../library/graph/heavy_light_decomposition.hpp"
#include "../../library/data_structure/monoid_segtree.hpp"
#include "../../library/graph/graph.hpp"

#include <algorithm>
#include <cassert>
#include <queue>
#include <random>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

struct Concat {
  using value_type = std::string;
  static value_type op(value_type a, value_type b) { return a + b; }
  static value_type id() { return {}; }
};

template <class H, class S>
concept CanMakeFold = requires(H&& h, S& seg) {
  cp::make_hld_path_fold<Concat>(std::forward<H>(h), seg);
};

using Bidi = cp::BidirectionalMonoid<Concat>;
using TestSeg = cp::Segtree<Bidi>;
using TestFold = cp::HldPathFold<Concat, TestSeg>;
static_assert(std::is_constructible_v<TestFold, const cp::HeavyLightDecomposition&, TestSeg&>);
static_assert(!std::is_constructible_v<TestFold, cp::HeavyLightDecomposition&&, TestSeg&>);
static_assert(!std::is_constructible_v<TestFold, const cp::HeavyLightDecomposition&&, TestSeg&>);
static_assert(CanMakeFold<cp::HeavyLightDecomposition&, TestSeg>);
static_assert(!CanMakeFold<cp::HeavyLightDecomposition, TestSeg>);
static_assert(!CanMakeFold<const cp::HeavyLightDecomposition, TestSeg>);

std::vector<int> path(const std::vector<std::vector<int>>& graph, int start, int goal) {
  std::vector<int> parent(graph.size(), -1);
  std::queue<int> q;
  parent[start] = start;
  q.push(start);
  while (!q.empty()) {
    int v = q.front(); q.pop();
    for (int to : graph[v]) if (parent[to] == -1) { parent[to] = v; q.push(to); }
  }
  std::vector<int> result;
  for (int v = goal; v != start; v = parent[v]) result.push_back(v);
  result.push_back(start);
  std::reverse(result.begin(), result.end());
  return result;
}

std::vector<int> expand(const cp::HeavyLightDecomposition& hld,
                        const std::vector<cp::HldSegment>& segments) {
  std::vector<int> result;
  for (auto segment : segments) {
    assert(0 <= segment.l && segment.l < segment.r && segment.r <= hld.size());
    if (segment.reversed) {
      for (int i = segment.r; i-- > segment.l;) result.push_back(hld.vertex(i));
    } else {
      for (int i = segment.l; i < segment.r; ++i) result.push_back(hld.vertex(i));
    }
  }
  return result;
}

void check_tree(const std::vector<std::vector<int>>& graph, int root, std::mt19937& rng) {
  const int n = static_cast<int>(graph.size());
  cp::HeavyLightDecomposition hld(graph, root);
  std::vector<int> expected_parent(n, -2), expected_depth(n, 0);
  expected_parent[root] = -1;
  std::queue<int> q;
  q.push(root);
  while (!q.empty()) {
    int v = q.front(); q.pop();
    for (int to : graph[v]) if (expected_parent[to] == -2) {
      expected_parent[to] = v;
      expected_depth[to] = expected_depth[v] + 1;
      q.push(to);
    }
  }
  Graph<int> concrete(n);
  for (int v = 0; v < n; ++v) for (int to : graph[v]) if (v < to) concrete.add_edge(v, to);
  cp::HeavyLightDecomposition hld_graph(concrete, root);
  assert(hld_graph.size() == n && hld_graph.root() == root);
  using B = cp::BidirectionalMonoid<Concat>;
  cp::Segtree<B> vertex_seg(n), edge_seg(n);
  auto vertices = cp::make_hld_path_fold<Concat>(hld, vertex_seg);
  auto edges = cp::make_hld_path_fold<Concat>(hld, edge_seg);
  std::vector<std::string> value(n);
  std::vector<std::vector<std::string>> directed(n, std::vector<std::string>(n));
  for (int v = 0; v < n; ++v) {
    value[v] = std::string(1, static_cast<char>('A' + v % 26));
    vertices.set_vertex(v, value[v]);
    for (int to : graph[v]) directed[v][to] = std::string(1, static_cast<char>('a' + rng() % 26));
  }
  for (int v = 0; v < n; ++v) if (hld.parent(v) != -1) {
    int p = hld.parent(v);
    edges.set_edge(v, p, directed[v][p], directed[p][v]);
  }
  for (int iter = 0; iter < 3; ++iter) {
    for (int v = 0; v < n; ++v) {
      assert(hld.vertex(hld.index(v)) == v);
      assert(hld.parent(v) == expected_parent[v] && hld.depth(v) == expected_depth[v]);
      assert(hld_graph.lca(v, root) == root);
      auto [l, r] = hld.subtree_vertices(v);
      auto [el, er] = hld.subtree_edges(v);
      assert(l == hld.index(v) && r - l == hld.subtree_size(v));
      assert(el == l + 1 && er == r);
      for (int x = 0; x < n; ++x) {
        int cur = x;
        while (cur != -1 && cur != v) cur = expected_parent[cur];
        assert((l <= hld.index(x) && hld.index(x) < r) == (cur == v));
      }
      for (int u = 0; u < n; ++u) {
        auto model = path(graph, u, v);
        assert(expand(hld, hld.path_vertices(u, v)) == model);
        std::vector<cp::HldSegment> vertex_callback;
        hld.for_each_path_vertices(u, v, [&](auto segment) { vertex_callback.push_back(segment); });
        assert(expand(hld, vertex_callback) == model);
        auto edge_vertices = model;
        for (std::size_t i = 1; i < model.size(); ++i) {
          int a = model[i - 1], b = model[i];
          edge_vertices[i - 1] = expected_parent[a] == b ? a : b;
        }
        edge_vertices.pop_back();
        assert(expand(hld, hld.path_edges(u, v)) == edge_vertices);
        std::vector<cp::HldSegment> edge_callback;
        hld.for_each_path_edges(u, v, [&](auto segment) { edge_callback.push_back(segment); });
        assert(expand(hld, edge_callback) == edge_vertices);
        std::string vertex_value, edge_value;
        for (int x : model) vertex_value += value[x];
        for (std::size_t i = 1; i < model.size(); ++i) edge_value += directed[model[i - 1]][model[i]];
        assert(vertices.vertices(u, v) == vertex_value);
        assert(edges.edges(u, v) == edge_value);
        int expected_lca = model[0];
        for (int x : model) if (expected_depth[x] < expected_depth[expected_lca]) expected_lca = x;
        assert(hld.lca(u, v) == expected_lca);
      }
    }
    int v = static_cast<int>(rng() % static_cast<unsigned>(n));
    value[v] = "X" + std::to_string(iter);
    vertices.set_vertex(v, value[v]);
    if (n > 1) {
      int child = root == 0 ? 1 : 0;
      if (child == root) child = 1;
      int p = hld.parent(child);
      directed[p][child] = "down" + std::to_string(iter);
      directed[child][p] = "up" + std::to_string(iter);
      edges.set_edge(p, child, directed[p][child], directed[child][p]);
    }
  }
}

int main() {
  std::mt19937 rng(420420);
  check_tree({{}}, 0, rng);
  check_tree({{1, 2, 3}, {0}, {0}, {0}}, 0, rng);
  for (int n = 2; n <= 30; ++n) {
    std::vector<std::vector<int>> graph(n);
    for (int v = 1; v < n; ++v) {
      int p = static_cast<int>(rng() % static_cast<unsigned>(v));
      graph[v].push_back(p); graph[p].push_back(v);
    }
    check_tree(graph, static_cast<int>(rng() % static_cast<unsigned>(n)), rng);
  }
  // Construction must not use the call stack on a long chain.
  constexpr int n = 200000;
  std::vector<std::vector<int>> chain(n);
  for (int i = 1; i < n; ++i) { chain[i - 1].push_back(i); chain[i].push_back(i - 1); }
  cp::HeavyLightDecomposition hld(chain);
  assert(hld.lca(n - 1, n / 2) == n / 2);
  assert(hld.subtree_size(n / 2) == n - n / 2);
}
