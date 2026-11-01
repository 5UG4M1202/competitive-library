#pragma once
#include "graph.hpp"
#include <algorithm>
#include <cassert>
#include <functional>
#include <iterator>
#include <limits>
#include <numeric>
#include <queue>
#include <set>
#include <tuple>
#include <utility>
#include <vector>

// Closely matches ei1333/library k-shortest-path.hpp; historical source unconfirmed.
// Comparison: https://github.com/ei1333/library/blob/088b4c8b49f85194099305d998e7113dc3d5dd2e/graph/shortest-path/k-shortest-path.hpp
// Comparison source license: Unlicense. See THIRD_PARTY_NOTICES.md.
template <typename T>
std::vector<std::pair<T, std::vector<int> > > k_shortest_path(const Graph<T> &g, int s, int t, int k) {
  assert(s != t);
  int N = (int)g.size();
  assert(0 <= s && s < N && 0 <= t && t < N);
  if (k <= 0) return {};
  int M = 0;
  for (int i = 0; i < N; i++) M += (int)g[i].size();
  std::vector<int> latte(M), malta(M);
  std::vector<T> cost(M);
  for (int i = 0; i < N; i++) {
    for (auto &e : g[i]) {
      latte[e.idx] = i;
      malta[e.idx] = e.to;
      cost[e.idx] = e.cost;
    }
  }
  const auto INF_ = std::numeric_limits<T>::max();
  std::vector<int> dame(M, -1);
  int timestamp = 0;
  // dijkstra
  auto shortest_path = [&](std::vector<T> &dist, std::vector<int> &from, std::vector<int> &id, int st) {
    using Pi = std::pair<T, int>;
    std::priority_queue<Pi, std::vector<Pi>, std::greater<> > que;
    que.emplace(dist[st], st);
    while (!que.empty()) {
      T cost;
      int idx;
      std::tie(cost, idx) = que.top();
      que.pop();
      if (dist[idx] < cost) continue;
      if (idx == t) return;
      for (auto &e : g[idx]) {
        if (e.cost > std::numeric_limits<T>::max() - cost) continue;
        auto next_cost = cost + e.cost;
        if (dist[e.to] <= next_cost) continue;
        if (dame[e.idx] == timestamp) continue;
        dist[e.to] = next_cost;
        from[e.to] = idx;
        id[e.to] = e.idx;
        que.emplace(dist[e.to], e.to);
      }
    }
  };
  auto restore = [](const std::vector<int> &es, const std::vector<int> &vs, int from,
                    int to) {
    std::vector<int> tap;
    while (to != from) {
      tap.emplace_back(es[to]);
      to = vs[to];
    }
    std::reverse(std::begin(tap), std::end(tap));
    return tap;
  };

  std::vector<T> dist(g.size(), INF_);
  std::vector<int> from(g.size(), -1), id(g.size(), -1);
  dist[s] = 0;
  shortest_path(dist, from, id, s);
  if (dist[t] == INF_) return {};

  std::vector<std::pair<T, std::vector<int> > > A;
  std::set<std::pair<T, std::vector<int> > > B;
  A.emplace_back(dist[t], restore(id, from, s, t));

  for (int i = 1; i < k; i++) {
    dist.assign(g.size(), INF_);
    from.assign(g.size(), -1);
    id.assign(g.size(), -1);
    dist[s] = 0;
    std::vector<int> candidate(A.size());
    std::iota(std::begin(candidate), std::end(candidate), 0);
    auto &last_path = A.back().second;
    int cur = s;
    for (int j = 0; j < last_path.size(); j++) {
      for (auto &k : candidate) {
        if (j < A[k].second.size()) dame[A[k].second[j]] = timestamp;
      }
      std::vector<T> dist2{dist};
      std::vector<int> from2{from}, id2{id};
      shortest_path(dist2, from2, id2, cur);
      ++timestamp;
      if (dist2[t] != INF_) {
        auto path = restore(id2, from2, s, t);
        bool ok = true;
        for (auto &p : candidate) {
          if (path == A[p].second) {
            ok = false;
            break;
          }
        }
        if (ok) B.emplace(dist2[t], path);
      }
      std::vector<int> accept;
      for (auto &k : candidate) {
        if (j < A[k].second.size() && A[k].second[j] == last_path[j]) {
          accept.emplace_back(k);
        }
      }
      dist[malta[last_path[j]]] =
          dist[latte[last_path[j]]] + cost[last_path[j]];
      from[malta[last_path[j]]] = latte[last_path[j]];
      id[malta[last_path[j]]] = last_path[j];
      cur = malta[last_path[j]];
      candidate = std::move(accept);
    }
    if (B.size()) {
      A.emplace_back(*B.begin());
      B.erase(B.begin());
    }
  }
  return A;
}
