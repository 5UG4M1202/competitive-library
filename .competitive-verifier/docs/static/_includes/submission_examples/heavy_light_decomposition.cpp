#include <cassert>
#include <string>
#include <vector>

struct Concat {
  using value_type = std::string;
  static value_type op(value_type a, value_type b) { return a + b; }
  static value_type id() { return {}; }
};

int main() {
  std::vector<std::vector<int>> g{ {1}, {0, 2}, {1} };
  cp::HeavyLightDecomposition hld(g, 0);
  using B = cp::BidirectionalMonoid<Concat>;
  cp::Segtree<B> seg(3);
  auto paths = cp::make_hld_path_fold<Concat>(hld, seg);
  paths.set_edge(0, 1, "A", "a");
  paths.set_edge(2, 1, "b", "B");
  assert(paths.edges(0, 2) == "AB");
  assert(paths.edges(2, 0) == "ba");
  assert(paths.edges(1, 1).empty());
}
