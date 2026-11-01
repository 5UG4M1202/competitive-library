// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/graph/doubling.hpp"
#include <cassert>
#include <climits>
#include <random>
#include <vector>

int main() {
  Doubling<int> zero(std::vector<int>{0}, 0);
  assert(zero.jump(0, 0) == 0);
  Doubling<int> high(std::vector<int>{0}, LLONG_MAX);
  assert(high.jump(LLONG_MAX, 0) == 0);
  Doubling<int> empty(std::vector<int>{}, 0);
  assert(empty.jump(0, -1) == -1);
  std::mt19937 rng(7719);
  for (int trial = 0; trial < 100; ++trial) {
    int n = static_cast<int>(rng() % 15) + 1;
    std::vector<int> next(n);
    for (int& v : next) v = static_cast<int>(rng() % (n + 1)) - 1;
    Doubling<int> db(next, 127);
    for (int v = 0; v < n; ++v) for (int k = 0; k < 128; ++k) {
      int expected = v;
      for (int j = 0; j < k && expected != -1; ++j) expected = next[expected];
      assert(db.jump(k, v) == expected);
    }
    db.build(next, 0);
    for (int v = 0; v < n; ++v) assert(db.jump(0, v) == v);
  }
}
