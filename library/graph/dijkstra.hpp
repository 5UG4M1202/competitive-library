#pragma once
#include "graph.hpp"
#include <functional>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

// Extracted from legacy cpptemplate; historical provenance not established.
// Apache-2.0 applies only to rrrriki's copyrightable human contributions.
template <typename T>
std::vector<T> dijkstra(Graph<T> &g, int s) {
  std::vector<T> dist(g.size(), std::numeric_limits<T>::max());
  dist[s] = 0;
  std::priority_queue<std::pair<T, int>, std::vector<std::pair<T, int> >, std::greater<std::pair<T, int> > > pq;
  pq.emplace(0, s);
  while (!pq.empty()) {
    auto [d, v] = pq.top();
    pq.pop();
    if (dist[v] < d) continue;
    for (auto e : g[v]) {
      if (e.cost > std::numeric_limits<T>::max() - dist[v]) continue;
      if (dist[e.to] > dist[v] + e.cost) {
        dist[e.to] = dist[v] + e.cost;
        pq.emplace(dist[e.to], e.to);
      }
    }
  }
  return dist;
}
