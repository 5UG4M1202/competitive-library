// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/graph/k_shortest_path.hpp"
#include <algorithm>
#include <cassert>
#include <functional>
#include <limits>
#include <random>
#include <set>
#include <utility>
#include <vector>

int main() {
  Graph<long long> simple(2);
  simple.add_directed_edge(0, 1, 3);
  assert(k_shortest_path(simple, 0, 1, 0).empty());
  auto one = k_shortest_path(simple, 0, 1, 4);
  assert(one.size() == 1 && one[0].first == 3);
  const long long inf = std::numeric_limits<long long>::max();
  Graph<long long> extreme(3);
  extreme.add_directed_edge(0, 1, inf - 5);
  extreme.add_directed_edge(1, 2, 10);
  extreme.add_directed_edge(0, 2, 7);
  auto bounded = k_shortest_path(extreme, 0, 2, 3);
  assert(bounded.size() == 1 && bounded[0].first == 7);

  std::mt19937 rng(61917);
  for (int n = 2; n <= 6; ++n) for (int trial = 0; trial < 200; ++trial) {
    Graph<long long> g(n);
    int m = static_cast<int>(rng() % 10);
    for (int e = 0; e < m; ++e) {
      int u = static_cast<int>(rng() % n), v = static_cast<int>(rng() % n);
      long long w = rng() % 8;
      g.add_directed_edge(u, v, w);
    }
    std::vector<std::pair<long long, std::vector<int>>> expected;
    std::vector<bool> visited(n);
    std::vector<int> path;
    visited[0] = true;
    std::function<void(int, long long)> dfs = [&](int u, long long cost) {
      if (u == n - 1) { expected.emplace_back(cost, path); return; }
      for (const auto& edge : g[u]) {
        if (visited[edge.to]) continue;
        visited[edge.to] = true;
        path.push_back(edge.idx);
        dfs(edge.to, cost + edge.cost);
        path.pop_back();
        visited[edge.to] = false;
      }
    };
    dfs(0, 0);
    std::sort(expected.begin(), expected.end());
    const int k = static_cast<int>(rng() % 9);
    auto actual = k_shortest_path(g, 0, n - 1, k);
    const std::size_t count = std::min<std::size_t>(expected.size(), static_cast<std::size_t>(k));
    assert(actual.size() == count);
    std::set<std::vector<int>> seen;
    for (std::size_t i = 0; i < actual.size(); ++i) {
      assert(actual[i].first == expected[i].first);
      assert(seen.insert(actual[i].second).second);
      auto it = std::find(expected.begin(), expected.end(), actual[i]);
      assert(it != expected.end());
    }
  }
}
