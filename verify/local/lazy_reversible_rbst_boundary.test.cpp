// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index

#include <cassert>
#include <vector>

#include "../../library/data_structure/lazy_reversible_rbst.hpp"

namespace {

struct Summary {
  long long sum = 0;
  int length = 0;

  friend bool operator==(const Summary&, const Summary&) = default;
};

struct Add {
  long long value = 0;

  friend bool operator==(const Add&, const Add&) = default;
};

Summary combine(Summary left, Summary right) {
  return {left.sum + right.sum, left.length + right.length};
}

Summary map_add(Summary summary, Add add) {
  summary.sum += add.value * summary.length;
  return summary;
}

Add compose_add(Add old_action, Add new_action) {
  return {old_action.value + new_action.value};
}

Summary reverse_summary(Summary summary) { return summary; }

using Tree = LazyReversibleRBST<Summary, Add, combine, map_add, compose_add,
                                reverse_summary>;

Summary value(long long x) { return {x, 1}; }

void assert_values(Tree& tree, Tree::Ptr& root,
                   const std::vector<long long>& expected) {
  assert(tree.size(root) == static_cast<int>(expected.size()));
  for (int index = 0; index < static_cast<int>(expected.size()); ++index) {
    assert(tree.fold(root, index, index + 1) == value(expected[index]));
  }
}

void clear(Tree& tree, Tree::Ptr& root) {
  while (root != nullptr) tree.erase(root, 0);
}

}  // namespace

int main() {
  Tree tree;
  auto root = tree.make_tree();
  assert(tree.size(root) == 0);
  assert(tree.fold(root, 0, 0) == Summary{});
  tree.reverse(root, 0, 0);

  const std::vector<Summary> initial = {value(1), value(2), value(3), value(4)};
  root = tree.build(initial);
  assert(tree.fold(root, 0, 4) == (Summary{10, 4}));

  auto parts = tree.split(root, 2);
  assert(tree.size(parts.first) == 2);
  assert(tree.size(parts.second) == 2);
  assert(tree.fold(parts.first, 0, 2) == (Summary{3, 2}));
  assert(tree.fold(parts.second, 0, 2) == (Summary{7, 2}));
  root = tree.merge(parts.first, parts.second);

  tree.apply(root, 1, 4, Add{5});
  assert_values(tree, root, {1, 7, 8, 9});

  tree.reverse(root, 0, 4);
  assert_values(tree, root, {9, 8, 7, 1});

  tree.insert(root, 2, value(10));
  assert_values(tree, root, {9, 8, 10, 7, 1});

  tree.erase(root, 1);
  assert_values(tree, root, {9, 10, 7, 1});
  assert(tree.fold(root, 1, 3) == (Summary{17, 2}));

  clear(tree, root);
  assert(root == nullptr);
  return 0;
}
