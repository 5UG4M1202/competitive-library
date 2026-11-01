// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/graph/dijkstra.hpp"
#include "../../library/graph/kruskal.hpp"
#include <algorithm>
#include <cassert>
#include <limits>
#include <random>
#include <sstream>
#include <tuple>
#include <vector>

int main() {
  const long long inf = std::numeric_limits<long long>::max();
  Graph<long long> extreme(3);
  extreme.add_directed_edge(0, 1, inf - 5);
  extreme.add_directed_edge(1, 2, 10);
  extreme.add_directed_edge(0, 2, 7);
  assert(dijkstra(extreme, 0)[2] == 7);
  Graph<int> default_graph;
  assert(default_graph.size() == 0 && default_graph.es == 0);
  Graph<int> edges(2);
  edges.add_edge(0, 1, 4);
  assert(edges.es == 1 && edges[0][0].idx == edges[1][0].idx);
  assert(static_cast<int>(edges[0][0]) == 1);
  std::ostringstream out;
  out << edges[0][0];
  assert(out.str() == "1");
  Graph<int> input(3);
  std::istringstream in("1 2 5\n2 3 8\n");
  auto* oldin = std::cin.rdbuf(in.rdbuf());
  input.read(2, -1, true, true);
  std::cin.rdbuf(oldin);
  assert(input.es == 2 && input[0][0].to == 1 && input[0][0].cost == 5);
  assert(input[1][0].to == 2 && input[1][0].cost == 8 && input[2].empty());

  std::mt19937 rng(1439);
  for (int n = 1; n <= 6; ++n) for (int trial = 0; trial < 100; ++trial) {
    Graph<long long> g(n);
    std::vector<std::vector<long long>> dist(n, std::vector<long long>(n, inf));
    for (int i = 0; i < n; ++i) dist[i][i] = 0;
    for (int e = 0; e < 20; ++e) {
      int u = static_cast<int>(rng() % n), v = static_cast<int>(rng() % n);
      long long w = rng() % 21;
      g.add_directed_edge(u, v, w);
      dist[u][v] = std::min(dist[u][v], w);
    }
    for (int k = 0; k < n; ++k) for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j)
        if (dist[i][k] != inf && dist[k][j] != inf)
          dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
    for (int s = 0; s < n; ++s) assert(dijkstra(g, s) == dist[s]);
  }
  Graph<long long> forest(5);
  forest.add_edge(0, 1, -2);
  forest.add_edge(1, 2, 3);
  forest.add_edge(0, 2, 5);
  forest.add_edge(3, 4, 7);
  assert(kruskal(forest) == 8);
  assert(kruskal(forest, "max") == 15);
  Graph<int> no_edges(0);
  assert(kruskal(no_edges) == 0);

  // Enumerate every acyclic spanning subset for small graphs, including
  // disconnected graphs, loops, parallel edges and negative costs.
  for (int n = 1; n <= 5; ++n) for (int trial = 0; trial < 100; ++trial) {
    Graph<long long> g(n);
    std::vector<std::tuple<int, int, long long>> input;
    const int m = static_cast<int>(rng() % 8);
    std::vector<std::vector<bool>> original(n, std::vector<bool>(n));
    for (int i = 0; i < n; ++i) original[i][i] = true;
    for (int e = 0; e < m; ++e) {
      int u = static_cast<int>(rng() % n), v = static_cast<int>(rng() % n);
      long long w = static_cast<int>(rng() % 17) - 8;
      input.emplace_back(u, v, w);
      g.add_edge(u, v, w);
      original[u][v] = original[v][u] = true;
    }
    for (int k = 0; k < n; ++k) for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j) original[i][j] = original[i][j] || (original[i][k] && original[k][j]);
    long long best_min = inf, best_max = -inf;
    for (int mask = 0; mask < (1 << m); ++mask) {
      std::vector<std::vector<bool>> reach(n, std::vector<bool>(n));
      for (int i = 0; i < n; ++i) reach[i][i] = true;
      long long weight = 0;
      bool cycle = false;
      for (int e = 0; e < m; ++e) if ((mask >> e) & 1) {
        auto [u, v, w] = input[e];
        if (reach[u][v]) { cycle = true; break; }
        reach[u][v] = reach[v][u] = true;
        for (int k = 0; k < n; ++k) for (int i = 0; i < n; ++i)
          for (int j = 0; j < n; ++j) reach[i][j] = reach[i][j] || (reach[i][k] && reach[k][j]);
        weight += w;
      }
      if (!cycle && reach == original) {
        best_min = std::min(best_min, weight);
        best_max = std::max(best_max, weight);
      }
    }
    assert(kruskal(g) == best_min && kruskal(g, "max") == best_max);
  }
}
