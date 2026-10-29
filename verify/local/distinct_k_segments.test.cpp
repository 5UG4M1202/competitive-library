// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
// Intended-correctness: brute-force DP comparison and boundary cases.
#include <algorithm>
#include <cassert>
#include <random>
#include <set>
#include <vector>
#include "../../library/data_structure/distinct_k_segments.hpp"

long long brute(const std::vector<int>& a, int k) {
  const int n = static_cast<int>(a.size());
  k = std::min(n, std::max(1, k));
  std::vector<std::vector<int>> dp(k + 1, std::vector<int>(n + 1, -1000000));
  dp[0][0] = 0;
  for (int j = 1; j <= k; ++j) for (int i = 1; i <= n; ++i) {
    std::set<int> seen;
    for (int p = i - 1; p >= 0; --p) {
      seen.insert(a[p]);
      dp[j][i] = std::max(dp[j][i], dp[j - 1][p] + static_cast<int>(seen.size()));
    }
  }
  return dp[k][n];
}
int main() {
  assert(DistinctKSegments(std::vector<int>{}, 1).solve() == 0);
  assert(DistinctKSegments(std::vector<int>{0, 1, 0}, 2).solve() == 3);
  std::mt19937 rng(20261029);
  for (int n = 1; n <= 9; ++n) for (int k = -1; k <= n + 1; ++k)
    for (int t = 0; t < 200; ++t) {
      std::vector<int> a(n);
      for (int& x : a) x = static_cast<int>(rng() % 4);
      assert(DistinctKSegments(a, k).solve() == brute(a, k));
    }
}
