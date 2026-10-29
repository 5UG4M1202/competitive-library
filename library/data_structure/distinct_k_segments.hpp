#pragma once

// The user reports creating legacy @DistinctMax with AI assistance.
// Copyright 2026 rrrriki. Copyrightable human-authored contributions are
// licensed under Apache-2.0; this provenance report does not guarantee rights
// in every generated fragment. See LICENSE-SCOPE.md and THIRD_PARTY_NOTICES.md.
// @see https://atcoder.jp/contests/abc397/submissions/71461138
#include <algorithm>
#include <vector>
#include <atcoder/lazysegtree>

template <class Int = int>
struct DistinctKSegments {
 private:
  using S = long long;
  using F = long long;
  static S op(S a, S b) { return std::max(a, b); }
  static S e() { return 0; }
  static S mapping(F f, S x) { return f + x; }
  static F composition(F f, F g) { return f + g; }
  static F id() { return 0; }
  using seg_t = atcoder::lazy_segtree<S, &DistinctKSegments::op, &DistinctKSegments::e,
      F, &DistinctKSegments::mapping, &DistinctKSegments::composition, &DistinctKSegments::id>;
  int N, K;
  std::vector<Int> a;
  std::vector<seg_t> seg;
  std::vector<std::vector<long long>> dp;
  std::vector<int> last;
 public:
  DistinctKSegments(const std::vector<Int>& a_compressed, int K_)
      : N((int)a_compressed.size()), K(K_), a(a_compressed) {
    if (K < 1) K = 1;
    if (K > N) K = N;
    int mx = 0;
    for (auto x : a) mx = std::max(mx, (int)x);
    last.assign(mx + 1, 0);
    seg.assign(K, seg_t(N + 1));
    dp.assign(K + 1, std::vector<long long>(N + 1));
  }
  long long solve() {
    for (int i = 1; i <= N; i++) {
      int v = (int)a[i - 1];
      for (int j = 1; j <= K; j++) {
        seg[j - 1].apply(last[v], i, 1);
        dp[j][i] = seg[j - 1].prod(0, i);
        if (j < K) seg[j].set(i, dp[j][i]);
      }
      last[v] = i;
    }
    return dp[K][N];
  }
};
