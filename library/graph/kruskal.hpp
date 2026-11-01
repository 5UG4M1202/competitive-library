#pragma once
#include "graph.hpp"
#include <algorithm>
#include <functional>
#include <string>
#include <atcoder/dsu>

// Extracted from legacy cpptemplate; historical provenance not established.
// Apache-2.0 applies only to rrrriki's copyrightable human contributions.
template <typename T>
T kruskal(Graph<T> &g, std::string s = "min") {
  T res = 0;
  int n = g.size();
  atcoder::dsu dsu(n);
  Edges<T> edges;
  for (int i = 0; i < n; i++) {
    for (auto e : g[i]) {
      edges.emplace_back(e);
    }
  }
  if (s == "max")
    std::sort(edges.begin(), edges.end(), std::greater<Edge<T> >());
  else
    std::sort(edges.begin(), edges.end());
  for (auto e : edges) {
    if (dsu.same(e.from, e.to)) continue;
    dsu.merge(e.from, e.to);
    res += e.cost;
  }
  return res;
}
