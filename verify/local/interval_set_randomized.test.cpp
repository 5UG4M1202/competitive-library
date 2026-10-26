// competitive-verifier: STANDALONE

#include <algorithm>
#include <cassert>
#include <random>
#include <vector>

#include "../../library/data_structure/interval_set.hpp"

namespace {

struct ExpectedNode {
  int l;
  int r;
  int value;
};

std::vector<ExpectedNode> build_expected(const std::vector<bool>& covered,
                                         const std::vector<int>& values) {
  std::vector<ExpectedNode> result;
  const int n = static_cast<int>(covered.size());
  for (int l = 0; l < n;) {
    if (!covered[l]) {
      ++l;
      continue;
    }
    int r = l + 1;
    while (r < n && covered[r] && values[r] == values[l]) ++r;
    result.push_back({l, r, values[l]});
    l = r;
  }
  return result;
}

}  // namespace

int main() {
  constexpr int n = 64;
  constexpr int identity = -1;
  std::mt19937 rng(0x00C0FFEEU);

  IntervalSet<int, int> actual(identity);
  IntervalSet<int, int> occupancy(identity);
  std::vector<bool> covered(n, false);
  std::vector<int> values(n, identity);

  for (int step = 0; step < 5000; ++step) {
    int l = static_cast<int>(rng() % (n + 1));
    int r = static_cast<int>(rng() % (n + 1));
    if (l > r) std::swap(l, r);

    if ((rng() & 3U) != 0U) {
      const int value = static_cast<int>(rng() % 4U);
      actual.update(l, r, value);
      occupancy.update(l, r, 0);
      for (int p = l; p < r; ++p) {
        covered[p] = true;
        values[p] = value;
      }
    } else {
      actual.erase(l, r);
      occupancy.erase(l, r);
      for (int p = l; p < r; ++p) {
        covered[p] = false;
        values[p] = identity;
      }
    }

    const auto expected = build_expected(covered, values);
    assert(actual.size() == expected.size());

    auto it = actual.begin();
    int expected_cover = 0;
    for (const auto& node : expected) {
      assert(it != actual.end());
      assert(it->l == node.l);
      assert(it->r == node.r);
      assert(it->val == node.value);
      expected_cover += node.r - node.l;
      ++it;
    }
    assert(it == actual.end());
    assert(actual.covered_all() == expected_cover);

    for (int p = 0; p < n; ++p) {
      assert(actual.covered(p) == covered[p]);
      assert(actual.get_val(p) == values[p]);
    }

    for (int query = 0; query < 4; ++query) {
      int ql = static_cast<int>(rng() % (n + 1));
      int qr = static_cast<int>(rng() % (n + 1));
      if (ql > qr) std::swap(ql, qr);

      int length = 0;
      for (int p = ql; p < qr; ++p) length += covered[p] ? 1 : 0;
      assert(actual.covered_length_in(ql, qr) == length);

      long long legacy_contained = 0;
      for (const auto& node : expected) {
        // Characterize the known bug rather than claiming intended
        // correctness: a node starting exactly at ql is skipped.
        legacy_contained += ql < node.l && node.r <= qr ? 1 : 0;
      }
      assert(actual.contained_intervals_in(ql, qr) == legacy_contained);

      bool one_interval = ql == qr;
      if (ql < qr) {
        for (const auto& node : expected) {
          if (node.l <= ql && qr <= node.r) one_interval = true;
        }
      }
      assert(actual.covered(ql, qr) == one_interval);
    }

    const int p = static_cast<int>(rng() % (n + 1));
    int mex = p;
    while (mex < n && covered[mex]) ++mex;
    assert(occupancy.get_mex(p) == mex);
  }

  return 0;
}
