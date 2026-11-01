#pragma once
#include <cassert>
#include <vector>

// Extracted from legacy cpptemplate; historical provenance not established.
// User's template reference: https://atcoder.jp/contests/abc377/submissions/71537117
// Apache-2.0 applies only to rrrriki's copyrightable human contributions.
template <typename T>
struct Doubling {
  int N;    // 頂点数
  int LOG;  // 格納する歩数のビット数
  int invalid;
  std::vector<std::vector<int>> db;

  /**
   * @param next   i の 1 歩先 next[i]
   * @param max_k  最大
   * @param invalid_ 行き止まり
   */
  Doubling(const std::vector<T>& next,
           long long max_k = (1LL << 61),
           int invalid_ = -1) {
    build(next, max_k, invalid_);
  }

  /// @brief テーブルを再構築したいときに呼ぶ
  void build(const std::vector<T>& next,
             long long max_k = (1LL << 60),
             int invalid_ = -1) {
    N = (int)next.size();
    invalid = invalid_;

    assert(max_k >= 0);
    LOG = 1;
    while (LOG < 63 && (static_cast<unsigned long long>(max_k) >> LOG) != 0) ++LOG;

    db.assign(LOG, std::vector<int>(N, invalid));
    // 1 歩先
    for (int i = 0; i < N; i++) {
      db[0][i] = next[i];
    }
    // 2^k 歩先
    for (int k = 1; k < LOG; k++) {
      for (int v = 0; v < N; v++) {
        int mid = db[k - 1][v];
        db[k][v] = (mid == invalid ? invalid : db[k - 1][mid]);
      }
    }
  }

  /// @brief v から k 歩進んだ先
  int jump(long long k, int v) const {
    assert(k >= 0 && (static_cast<unsigned long long>(k) >> LOG) == 0);
    assert(v == invalid || (0 <= v && v < N));
    for (int bit = 0; bit < LOG; bit++) {
      if (k & (1LL << bit)) {
        if (v == invalid) break;
        v = db[bit][v];
      }
    }
    return v;
  }
};
