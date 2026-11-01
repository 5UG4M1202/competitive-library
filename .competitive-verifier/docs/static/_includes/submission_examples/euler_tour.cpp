#include <iostream>
#include <vector>

int main() {
  Graph<long long> graph(4);
  graph.add_edge(0, 1, 2);
  graph.add_edge(0, 2, 3);
  graph.add_edge(1, 3, 5);
  std::vector<long long> vertex_weight = {10, 20, 30, 40};
  EulerTour<long long, long long> tour(graph, 0, vertex_weight);
  std::cout << tour.lca(3, 2) << ' ' << tour.query_edge(3, 2) << ' '
            << tour.query_vert(3, 2) << '\n';
  std::cout << tour.query_subtree_vert(1) << '\n';
  tour.add_cost_subtree_vert(1, 10, 2);
  std::cout << tour.query_subtree_vert(1) << ' ' << tour.query_vert(3, 2) << '\n';
}
