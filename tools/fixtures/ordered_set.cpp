// @snippet-definition@
#include <algorithm>
#include <cassert>
#include <iterator>
#include <random>
#include <set>

int main() {
  ordered_set_less<int> actual;
  std::set<int> expected;
  assert(actual.find_by_order(0) == actual.end());
  std::mt19937 rng(641);
  for (int step = 0; step < 5000; ++step) {
    int value = static_cast<int>(rng() % 101) - 50;
    if (rng() % 2) {
      assert(actual.insert(value).second == expected.insert(value).second);
    } else {
      assert(actual.erase(value) == expected.erase(value));
    }
    assert(actual.size() == expected.size());
    assert(actual.order_of_key(value) == static_cast<std::size_t>(std::distance(expected.begin(), expected.lower_bound(value))));
    std::size_t rank = 0;
    for (int x : expected) { auto it = actual.find_by_order(rank++); assert(it != actual.end() && *it == x); }
    assert(actual.find_by_order(rank) == actual.end());
  }
}
