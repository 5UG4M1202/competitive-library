// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/graph/euler_tour.hpp"
#include "../../library/graph/graph.hpp"
#include "../../library/graph/bipartite_graph.hpp"
#include <cassert>
#include <vector>

int main() {
  Graph<long long> g(3);
  g.add_edge(0, 1, 2);
  g.add_edge(1, 2, 5);
  EulerTour<long long, long long> tour(g, 0, std::vector<long long>{1, 2, 3});
  assert(tour.lca(0, 2) == 0);
  assert(tour.query_edge(0, 2) == 7);
  BipartiteGraph colors(g);
  assert(colors.is_bipartite() && colors[0] != colors[1] && colors[1] != colors[2]);
}
