#include <cassert>
struct Sum {
  using value_type = long long;
  static value_type id() { return 0; }
  static value_type op(value_type a, value_type b) { return a + b; }
};
int main() {
  cp::DynamicSegtree2D<Sum> tree(-100, 101, -100, 101);
  tree.set(1, 3, 4); tree.set(2, 5, 6); // 座標は事前登録不要
  assert(tree.prod(1, 3, 3, 6) == 10);
  tree.set(1, 3, tree.get(1, 3) + 2); // 一点加算
  assert(tree.get(1, 3) == 6);
}
