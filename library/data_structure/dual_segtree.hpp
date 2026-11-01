#pragma once
#include <cassert>
#include <vector>

// Interval propagation closely resembles ACL lazy_segtree (CC0-1.0).
// Historical source of this implementation is unconfirmed; see THIRD_PARTY_NOTICES.md.
inline int dual_segtree_ceil_pow2(int n) {
  int x = 0;
  while ((1U << x) < (unsigned int)(n)) x++;
  return x;
}
/// @brief 双対セグメント木 @tparam T セグメント木の型 @tparam composition セグメント木のマージ関数 @tparam id セグメント木の単位元
/// @fn apply 区間に作用を適用する @fn get 位置pの値を取得する
template <class F, F (*composition)(F, F), F (*id)()>
struct dual_segtree {
 public:
  /// @brief セグメント木を初期化する @param n サイズ
  explicit dual_segtree(int n) : dual_segtree(std::vector<F>(n, id())) {}

  /// @brief セグメント木を初期化する @param v vector<F>型の配列
  explicit dual_segtree(const std::vector<F> &v) : _n(int(v.size())) {
    log = dual_segtree_ceil_pow2(_n);
    size = 1 << log;
    lz = std::vector<F>(2 * size, id());
    for (int i = 0; i < _n; i++) lz[size + i] = v[i];
  }

  /// @brief  [l, r) の要素に f を作用させます O(logN) @param l 左端 @param r 右端 @param f 作用素
  void apply(int l, int r, F f) {
    assert(0 <= l && l <= r && r <= _n);
    if (l == r) return;

    l += size;
    r += size;

    for (int i = log; i >= 1; i--) {                 // 遅延評価
      if (((l >> i) << i) != l) push(l >> i);        // lがiの倍数でない場合は、lを親に移動
      if (((r >> i) << i) != r) push((r - 1) >> i);  // rがiの倍数でない場合は、rを親に移動
    }
    while (l < r) {
      if (l & 1) all_apply(l++, f);  // lが奇数の場合は、lに作用を適用してからlをインクリメント
      if (r & 1) all_apply(--r, f);  // rが奇数の場合は、rをデクリメントしてからrに作用を適用
      l >>= 1;                       // lを親に移動
      r >>= 1;                       // rを親に移動
    }
  }
  /// @brief 位置pの値を取得する @param p 位置
  F get(int p) {
    assert(0 <= p && p < _n);
    p += size;
    for (int i = log; i >= 1; i--) push(p >> i);
    return lz[p];
  }

 private:
  int _n, size, log;
  std::vector<F> lz;

  /// @brief 作用素を遅延評価する @param i 位置 @param f 作用素
  void all_apply(int i, F f) {
    lz[i] = composition(f, lz[i]);
  }
  /// @brief 作用素を遅延評価する @param i 位置
  void push(int i) {
    assert(i < size);
    all_apply(2 * i, lz[i]);
    all_apply(2 * i + 1, lz[i]);
    lz[i] = id();
  }
};
