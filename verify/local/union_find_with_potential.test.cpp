// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/data_structure/union_find_with_potential.hpp"

#include <cassert>
#include <queue>
#include <random>
#include <utility>
#include <vector>

int main() {
  UnionFindWithPotential<long long> empty(0);
  assert(empty.dat.empty());
  UnionFindWithPotential<long long> one(1);
  assert(one.size(0) == 1 && one.potential(0) == 0);
  assert(one.merge(0, 0, 0));
  assert(!one.merge(0, 0, 1));

  constexpr int n = 18;
  UnionFindWithPotential<long long> uf(n);
  std::vector<std::vector<std::pair<int, long long>>> graph(n);
  std::mt19937 rng(81297);
  for (int step = 0; step < 1500; ++step) {
    const int x = static_cast<int>(rng() % n);
    const int y = static_cast<int>(rng() % n);
    const long long p = static_cast<long long>(rng() % 31) - 15;
    std::vector<int> seen(n);
    std::vector<long long> value(n);
    std::queue<int> q;
    q.push(x);
    seen[x] = 1;
    int count = 0;
    while (!q.empty()) {
      const int u = q.front(); q.pop(); ++count;
      for (auto [v, delta] : graph[u]) {
        if (!seen[v]) {
          seen[v] = 1;
          value[v] = value[u] + delta;
          q.push(v);
        }
      }
    }
    assert(uf.size(x) == count);
    for (int v = 0; v < n; ++v) {
      assert(uf.same(x, v) == static_cast<bool>(seen[v]));
      if (seen[v]) assert(uf.diff(x, v) == -value[v]);
    }
    const bool expected = !seen[y] || -value[y] == p;
    assert(uf.merge(x, y, p) == expected);
    if (!seen[y]) {
      graph[x].push_back({y, -p});
      graph[y].push_back({x, p});
    }
  }
}
