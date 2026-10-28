// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index

#include <cassert>
#include <cstddef>
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

int main() {
  Graph<long long> g(5);
  g.add_edge(0, 1, 2);
  g.add_edge(0, 2, 3);
  g.add_edge(1, 3, 5);
  g.add_edge(1, 4, 7);
  std::vector<long long> vertex = {10, 20, 30, 40, 50};
  EulerTour<long long, long long> tour(g, 0, vertex);

  assert(tour.lca(3, 4) == 1);
  assert(tour.lca(3, 2) == 0);
  assert(tour.lca(1, 3) == 1);
  assert(tour.get_depth(0) == 0);
  assert(tour.get_depth(3) == 2);

  assert(tour.query_edge(3, 4) == 12);
  assert(tour.query_edge(3, 2) == 10);
  assert(tour.query_edge(3, 3) == 0);
  assert(tour.query_vert(3, 4) == 90);  // 40 + 50, LCA 1 excluded.
  assert(tour.query_vert(3, 2) == 90);  // 40 + 20 + 30, LCA 0 excluded.
  assert(tour.query_vert(1, 3) == 40);
  assert(tour.query_vert(3, 3) == 0);

  assert(tour.query_subtree_edge(1) == 12);
  assert(tour.query_subtree_edge(0) == 17);
  assert(tour.query_subtree_edge(3) == 0);
  assert(tour.query_subtree_vert(1) == 110);
  assert(tour.query_subtree_vert(0) == 150);
  assert(tour.get_edge(0, 1) == 2);
  assert(tour.get_vert(0) == 10);

  tour.update_edge(1, 3, 11, 2);
  assert(tour.query_edge(3, 4) == 18);
  assert(tour.query_subtree_edge(1) == 18);
  tour.add_cost_edge(1, 4, 3, 2);
  assert(tour.get_edge(1, 4) == 10);
  assert(tour.query_edge(3, 4) == 21);
  assert(tour.query_subtree_edge(1) == 21);

  tour.update_vert(3, 44, 2);
  assert(tour.query_vert(3, 4) == 94);
  assert(tour.query_subtree_vert(1) == 114);
  tour.add_cost_vert(4, 2, 2);
  assert(tour.get_vert(4) == 52);
  tour.add_cost_subtree_vert(1, 10, 2);
  assert(tour.query_subtree_vert(1) == 146);
  assert(tour.query_vert(3, 4) == 116);

  tour.add_cost_subtree_edge(1, 2, 2);
  assert(tour.query_subtree_edge(1) == 25);
  assert(tour.query_edge(3, 4) == 25);

  tour.update_vert(0, 100, 2);
  assert(tour.get_vert(0, 0) == 100);
  assert(tour.get_vert(0, 1) == 100);
  assert(tour.query_subtree_vert(0) == 276);
  assert(tour.query_vert(3, 2) == 114);  // Root (the LCA) remains excluded.
  tour.add_cost_vert(0, 5, 2);
  assert(tour.get_vert(0, 0) == 105);
  assert(tour.get_vert(0, 1) == 105);
  assert(tour.query_subtree_vert(0) == 281);
  assert(tour.query_vert(3, 2) == 114);

  Graph<long long> branch(4);
  branch.add_edge(0, 1, 0);
  branch.add_edge(1, 2, 0);
  branch.add_edge(1, 3, 0);
  std::vector<long long> zero_vertex(4, 0);
  EulerTour<long long, long long> vertex_add(branch, 0, zero_vertex);
  vertex_add.add_cost_subtree_vert(1, 10, 1);
  assert(vertex_add.query_vert(2, 3) == 20);

  EulerTour<long long, long long> edge_add(branch, 0, zero_vertex);
  edge_add.add_cost_subtree_edge(1, 10, 1);
  assert(edge_add.query_edge(2, 3) == 20);

  Graph<long long> singleton_graph(1);
  std::vector<long long> singleton_weight = {7};
  EulerTour<long long, long long> singleton(singleton_graph, 0,
                                             singleton_weight);
  assert(singleton.query_subtree_vert(0) == 7);
  singleton.update_vert(0, 11, 2);
  assert(singleton.get_vert(0, 0) == 11);
  assert(singleton.get_vert(0, 1) == 11);
  assert(singleton.query_subtree_vert(0) == 11);
  singleton.add_cost_vert(0, 4, 2);
  assert(singleton.get_vert(0, 0) == 15);
  assert(singleton.get_vert(0, 1) == 15);
  assert(singleton.query_subtree_vert(0) == 15);
  assert(singleton.query_vert(0, 0) == 0);
}
