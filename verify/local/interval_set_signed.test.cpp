// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/data_structure/interval_set.hpp"
#include <algorithm>
#include <cassert>
#include <random>
#include <vector>

int main() {
  constexpr int origin = -24, length = 48;
  std::mt19937 rng(927);
  for (int trial = 0; trial < 40; ++trial) {
    IntervalSet<int, int> actual(-1);
    std::vector<int> model(length, -1);
    long long weighted = 0;
    auto add = [&](int l, int r, int v) { assert(l < r); weighted += 1LL * (r - l) * v; };
    auto del = [&](int l, int r, int v) { assert(l < r); weighted -= 1LL * (r - l) * v; };
    for (int step = 0; step < 800; ++step) {
      int l = static_cast<int>(rng() % (length + 1));
      int r = static_cast<int>(rng() % (length + 1));
      if (l > r) std::swap(l, r);
      if (rng() % 4 == 0) {
        actual.erase(origin + l, origin + r, add, del);
        std::fill(model.begin() + l, model.begin() + r, -1);
      } else {
        int v = static_cast<int>(rng() % 4);
        actual.update(origin + l, origin + r, v, add, del);
        std::fill(model.begin() + l, model.begin() + r, v);
      }
      int count = 0;
      long long expected_weight = 0;
      for (int i = 0; i < length; ++i) {
        assert(actual.get_val(origin + i) == model[i]);
        assert(actual.covered(origin + i) == (model[i] != -1));
        if (model[i] != -1) { ++count; expected_weight += model[i]; }
        int j = i;
        while (j < length && model[j] != -1) ++j;
        assert(actual.get_mex(origin + i) == origin + j);
      }
      assert(actual.covered_all() == count && weighted == expected_weight);
      int last_end = origin;
      for (const auto& node : actual) {
        assert(node.l < node.r && last_end <= node.l);
        last_end = node.r;
      }
    }
  }
}
