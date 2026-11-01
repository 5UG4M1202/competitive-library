// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/graph/bipartite_graph.hpp"
#include <cassert>
#include <queue>
#include <random>
#include <vector>

struct Edge { int from, to; };
using Graph = std::vector<std::vector<Edge>>;

bool reference(const Graph& g) {
  std::vector<int> colors(g.size(), -1);
  for (int s = 0; s < static_cast<int>(g.size()); ++s) {
    if (colors[s] != -1) continue;
    colors[s] = 0;
    std::queue<int> q;
    q.push(s);
    while (!q.empty()) {
      int u = q.front(); q.pop();
      for (const auto& e : g[u]) {
        if (colors[e.to] == -1) { colors[e.to] = colors[u] ^ 1; q.push(e.to); }
        else if (colors[e.to] == colors[u]) return false;
      }
    }
  }
  return true;
}

int main() {
  Graph empty;
  BipartiteGraph no_vertices(empty);
  assert(no_vertices.is_bipartite());
  Graph self(1);
  self[0].push_back({0, 0});
  BipartiteGraph not_bipartite(self);
  assert(!not_bipartite.is_bipartite());
  Graph directed(3);
  directed[0].push_back({0, 1});
  directed[2].push_back({2, 1});
  BipartiteGraph weak(directed);
  assert(weak.is_bipartite());
  assert(weak[0] == weak[2] && weak[0] != weak[1]);

  std::mt19937 rng(2834);
  for (int n = 1; n <= 9; ++n) for (int trial = 0; trial < 300; ++trial) {
    Graph g(n);
    for (int u = 0; u < n; ++u) for (int v = u; v < n; ++v) {
      if (rng() % 5 != 0) continue;
      g[u].push_back({u, v});
      if (u != v) g[v].push_back({v, u});
    }
    BipartiteGraph actual(g);
    bool expected = reference(g);
    assert(actual.is_bipartite() == expected);
    if (!expected) continue;
    for (int u = 0; u < n; ++u) {
      assert(actual[u] == 0 || actual[u] == 1);
      for (const auto& e : g[u]) assert(actual[u] != actual[e.to]);
    }
  }
}
