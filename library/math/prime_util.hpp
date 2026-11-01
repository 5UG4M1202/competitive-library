#pragma once
#include <algorithm>
#include <cassert>
#include <map>
#include <numeric>
#include <vector>

// SPDX-License-Identifier: Apache-2.0
// The user confirms PrimeUtil was created using generative AI.
class PrimeUtil {
 private:
  /// @brief spf[x] = x の最小素因数 (x >= 2)、x が素数の場合は spf[x] = x となる。
  std::vector<int> spf;
  /// @brief 数値の最大値
  int maxN;

 public:
  /// @brief 1からnまでの最小素因数テーブル (spf) を構築 @param n 上限となる数 (1 <= n) @note 計算量はO(nloglogn)
  PrimeUtil(int n) : spf(n + 1), maxN(n) {
    // spf[i] を i で初期化 (i は素数かもしれないという仮定)
    std::iota(spf.begin(), spf.end(), 0);
    // エラトステネス類似の処理で spf を更新
    for (int i = 2; i * i <= n; i++) {
      if (spf[i] == i) {  // i が素数のとき
        for (int j = i * i; j <= n; j += i) {
          // まだ更新されていない(=素数とされている)なら最小素因数を i に書き換える
          if (spf[j] == j) {
            spf[j] = (int)i;
          }
        }
      }
    }
  }
  /// @brief xが素数かどうかを判定 @param x 素数判定したい整数 @return xが素数ならtrue、そうでないならfalse @note 計算量はO(1)
  bool is_prime(int x) const {
    if (x < 2) return false;
    assert(x <= maxN);     // x が上限を超えていないか確認
    return (spf[x] == x);  // spf[x] == x なら素数
  }
  /// @brief xの素因数分解を行う @param x 素因数分解したい整数 @return xの素因数分解結果 (素因数 -> 指数) のmap @note 計算量はO(logx)
  std::map<int, int> factor(int x) const {
    std::map<int, int> ret;
    if (x < 2) return ret;  // x=1などは空
    while (x > 1) {         // SPF を使って高速に分解
      int p = spf[x];
      ret[p]++;
      x /= p;
    }
    return ret;
  }
  /// @brief xの約数を列挙する @param x 約数を列挙したい整数 @param is_sorted 結果をソートするかどうか @return xの約数のvector @note 計算量はO(logx)
  std::vector<int> divisor(int x, bool is_sorted = true) const {
    // 素因数分解 O(logx)
    auto mp = factor(x);
    std::vector<int> ret;
    ret.emplace_back(1);
    // mpは (素数p -> 個数c) のmap
    for (auto &kv : mp) {
      int prime = kv.first;
      int count = kv.second;
      // 既存の約数に prime^1, prime^2, ... prime^count を掛けた新たな約数を追加
      int nowSize = (int)ret.size();
      long long base = 1;
      for (int i = 0; i < count; i++) {
        base *= prime;
        for (int j = 0; j < nowSize; j++) {
          ret.emplace_back(ret[j] * base);
        }
      }
    }
    if (is_sorted) std::sort(ret.begin(), ret.end());
    return ret;
  }
};
