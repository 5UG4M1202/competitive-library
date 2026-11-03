#include <cassert>
#include <utility>
#include <vector>
struct Sum {
  using value_type = long long;
  static value_type id() { return 0; }
  static value_type op(value_type a, value_type b) { return a + b; }
};
int main() {
  std::vector<std::pair<long long, long long>> points;
  points.emplace_back(1, 3); points.emplace_back(2, 5); points.emplace_back(2, 7);
  cp::SparseSegtree2D<long long, Sum> tree(points);
  tree.set(1, 3, 4); tree.set(2, 5, 6);
  assert(tree.prod(1, 3, 3, 6) == 10);
  tree.set(1, 3, 2); // 加算ではなく上書き
  assert(tree.get(1, 3) == 2);
}
