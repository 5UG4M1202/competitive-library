// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/misc/mo.hpp"
#include <cassert>
#include <deque>
#include <random>
#include <vector>

int main() {
  Mo empty(0);
  bool called = false;
  auto ignore = [](int) {};
  empty.run(ignore, ignore, ignore, ignore, [&](int) { called = true; });
  assert(!called);
  std::mt19937 rng(424242);
  for (int n = 0; n <= 60; ++n) {
    for (int trial = 0; trial < 20; ++trial) {
      Mo mo(n);
      std::vector<std::pair<int, int>> queries;
      for (int i = 0; i < 100; ++i) {
        int l = static_cast<int>(rng() % (n + 1));
        int r = static_cast<int>(rng() % (n + 1));
        if (l > r) std::swap(l, r);
        assert(mo.add_query(l, r) == i);
        queries.emplace_back(l, r);
      }
      std::deque<int> active;
      std::vector<int> visits(queries.size());
      mo.run([&](int i) { active.push_front(i); },
             [&](int i) { active.push_back(i); },
             [&](int i) { assert(!active.empty() && active.front() == i); active.pop_front(); },
             [&](int i) { assert(!active.empty() && active.back() == i); active.pop_back(); },
             [&](int id) {
               ++visits[id];
               auto [l, r] = queries[id];
               assert(static_cast<int>(active.size()) == r - l);
               for (int i = l; i < r; ++i) assert(active[i - l] == i);
             });
      for (int count : visits) assert(count == 1);
    }
  }
}
