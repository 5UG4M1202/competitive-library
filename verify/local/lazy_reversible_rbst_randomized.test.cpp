// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index

#include <algorithm>
#include <cassert>
#include <numeric>
#include <random>
#include <vector>

#include "../../library/data_structure/lazy_reversible_rbst.hpp"

namespace {

struct Summary {
  long long sum = 0;
  int length = 0;
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

long long expected_sum(const std::vector<long long>& values, int left,
                       int right) {
  return std::accumulate(values.begin() + left, values.begin() + right, 0LL);
}

void check(Tree& tree, Tree::Ptr& root,
           const std::vector<long long>& expected, std::mt19937& random) {
  const int size = static_cast<int>(expected.size());
  assert(tree.size(root) == size);
  for (int query = 0; query < 8; ++query) {
    int left = static_cast<int>(random() % (size + 1));
    int right = static_cast<int>(random() % (size + 1));
    if (left > right) std::swap(left, right);
    const Summary actual = tree.fold(root, left, right);
    assert(actual.sum == expected_sum(expected, left, right));
    assert(actual.length == right - left);
  }
}

void clear(Tree& tree, Tree::Ptr& root) {
  while (root != nullptr) tree.erase(root, 0);
}

}  // namespace

int main() {
  Tree tree;
  auto root = tree.make_tree();
  std::vector<long long> expected;
  std::mt19937 random(20261028);

  for (int step = 0; step < 5000; ++step) {
    const int size = static_cast<int>(expected.size());
    int operation = static_cast<int>(random() % 5);
    if (size == 0) operation = 0;
    if (size >= 50 && operation == 0) operation = 1;

    if (operation == 0) {
      const int position = static_cast<int>(random() % (size + 1));
      const long long inserted = static_cast<int>(random() % 201) - 100;
      tree.insert(root, position, value(inserted));
      expected.insert(expected.begin() + position, inserted);
    } else if (operation == 1) {
      const int position = static_cast<int>(random() % size);
      tree.erase(root, position);
      expected.erase(expected.begin() + position);
    } else {
      int left = static_cast<int>(random() % (size + 1));
      int right = static_cast<int>(random() % (size + 1));
      if (left > right) std::swap(left, right);

      if (operation == 2) {
        if (left == right) {
          right = std::min(size, left + 1);
          left = right - 1;
        }
        const long long delta = static_cast<int>(random() % 21) - 10;
        tree.apply(root, left, right, Add{delta});
        for (int index = left; index < right; ++index) expected[index] += delta;
      } else if (operation == 3) {
        tree.reverse(root, left, right);
        std::reverse(expected.begin() + left, expected.begin() + right);
      } else {
        const Summary actual = tree.fold(root, left, right);
        assert(actual.sum == expected_sum(expected, left, right));
        assert(actual.length == right - left);
      }
    }

    check(tree, root, expected, random);
    if (step % 97 == 0) {
      for (int index = 0; index < static_cast<int>(expected.size()); ++index) {
        const Summary actual = tree.fold(root, index, index + 1);
        assert(actual.sum == expected[index]);
        assert(actual.length == 1);
      }
    }
  }

  clear(tree, root);
  return 0;
}
