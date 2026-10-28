// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <random>
#include <utility>
#include <vector>

template <typename T = int>
struct Graph {
  struct Edge {
    int to;
    T cost;
  };
  std::vector<std::vector<Edge>> g;
  explicit Graph(int n) : g(n) {}
  std::size_t size() const { return g.size(); }
  std::vector<Edge>& operator[](int v) { return g[v]; }
  const std::vector<Edge>& operator[](int v) const { return g[v]; }
  void add_edge(int u, int v, T cost) {
    g[u].push_back({v, cost});
    g[v].push_back({u, cost});
  }
};

#include "../../library/graph/euler_tour.hpp"

namespace {

struct Model {
  int n;
  std::vector<int> parent, depth, tin, tout;
  std::vector<long long> vertex, parent_edge;
  int timer = 0;

  Model(const Graph<long long>& g, std::vector<long long> vertex_weight)
      : n(static_cast<int>(g.size())),
        parent(n, -1),
        depth(n),
        tin(n),
        tout(n),
        vertex(std::move(vertex_weight)),
        parent_edge(n) {
    dfs(g, 0, -1);
  }

  void dfs(const Graph<long long>& g, int v, int p) {
    parent[v] = p;
    tin[v] = timer++;
    for (auto e : g[v]) {
      if (e.to == p) continue;
      depth[e.to] = depth[v] + 1;
      parent_edge[e.to] = e.cost;
      dfs(g, e.to, v);
    }
    tout[v] = timer;
  }

  bool in_subtree(int root, int v) const {
    return tin[root] <= tin[v] && tin[v] < tout[root];
  }

  int lca(int a, int b) const {
    while (depth[a] > depth[b]) a = parent[a];
    while (depth[b] > depth[a]) b = parent[b];
    while (a != b) {
      a = parent[a];
      b = parent[b];
    }
    return a;
  }

  long long query_edge(int a, int b) const {
    const int c = lca(a, b);
    long long sum = 0;
    while (a != c) {
      sum += parent_edge[a];
      a = parent[a];
    }
    while (b != c) {
      sum += parent_edge[b];
      b = parent[b];
    }
    return sum;
  }

  long long query_vert(int a, int b) const {
    const int c = lca(a, b);
    long long sum = 0;
    while (a != c) {
      sum += vertex[a];
      a = parent[a];
    }
    while (b != c) {
      sum += vertex[b];
      b = parent[b];
    }
    return sum;
  }

  long long subtree_edge(int root) const {
    long long sum = 0;
    for (int v = 1; v < n; ++v)
      if (in_subtree(root, v) && v != root) sum += parent_edge[v];
    return sum;
  }

  long long subtree_vert(int root) const {
    long long sum = 0;
    for (int v = 0; v < n; ++v)
      if (in_subtree(root, v)) sum += vertex[v];
    return sum;
  }

  int child_for_edge(int u, int v) const {
    if (parent[u] == v) return u;
    assert(parent[v] == u);
    return v;
  }
};

}  // namespace

int main() {
  std::mt19937_64 rng(0x45554c4552544f55ULL);
  for (int trial = 0; trial < 45; ++trial) {
    const int n = 2 + static_cast<int>(rng() % 24);
    Graph<long long> g(n);
    std::vector<std::pair<int, int>> edges;
    for (int v = 1; v < n; ++v) {
      const int p = static_cast<int>(rng() % v);
      const long long w = static_cast<long long>(rng() % 31) - 15;
      g.add_edge(p, v, w);
      edges.push_back({p, v});
    }
    std::vector<long long> vertex(n);
    for (auto& x : vertex) x = static_cast<long long>(rng() % 31) - 15;

    EulerTour<long long, long long> tour(g, 0, vertex);
    Model model(g, vertex);
    for (int step = 0; step < 450; ++step) {
      const int op = static_cast<int>(rng() % 11);
      const int u = static_cast<int>(rng() % n);
      const int v = static_cast<int>(rng() % n);
      const long long x = static_cast<long long>(rng() % 25) - 12;
      if (op == 0) {
        assert(tour.lca(u, v) == model.lca(u, v));
        assert(tour.get_depth(u) == model.depth[u]);
      } else if (op == 1) {
        assert(tour.query_edge(u, v) == model.query_edge(u, v));
      } else if (op == 2) {
        assert(tour.query_vert(u, v) == model.query_vert(u, v));
      } else if (op == 3) {
        assert(tour.query_subtree_edge(u) == model.subtree_edge(u));
        assert(tour.query_subtree_vert(u) == model.subtree_vert(u));
      } else if (op == 4 || op == 5) {
        const auto [a, b] = edges[rng() % edges.size()];
        const int child = model.child_for_edge(a, b);
        if (op == 4) {
          tour.update_edge(a, b, x, 2);
          model.parent_edge[child] = x;
        } else {
          tour.add_cost_edge(a, b, x, 2);
          model.parent_edge[child] += x;
        }
        assert(tour.get_edge(a, b, 0) == model.parent_edge[child]);
        assert(tour.get_edge(a, b, 1) == model.parent_edge[child]);
      } else if (op == 6 || op == 7) {
        const int target = static_cast<int>(rng() % n);
        if (op == 6) {
          tour.update_vert(target, x, 2);
          model.vertex[target] = x;
        } else {
          tour.add_cost_vert(target, x, 2);
          model.vertex[target] += x;
        }
        assert(tour.get_vert(target, 0) == model.vertex[target]);
        assert(tour.get_vert(target, 1) == model.vertex[target]);
      } else if (op == 8) {
        tour.add_cost_subtree_edge(u, x, 2);
        for (int w = 1; w < n; ++w)
          if (model.in_subtree(u, w) && w != u) model.parent_edge[w] += x;
      } else if (op == 9) {
        tour.add_cost_subtree_vert(u, x, 2);
        for (int w = 0; w < n; ++w)
          if (model.in_subtree(u, w)) model.vertex[w] += x;
      } else {
        assert(tour.query_edge(u, v) == model.query_edge(u, v));
        assert(tour.query_vert(u, v) == model.query_vert(u, v));
        assert(tour.query_subtree_edge(u) == model.subtree_edge(u));
        assert(tour.query_subtree_vert(u) == model.subtree_vert(u));
      }
    }
  }
}
