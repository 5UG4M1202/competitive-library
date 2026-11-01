#pragma once
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <random>
#include <string>
#include <vector>

/**
 * @brief Rolling-Hash(ローリングハッシュ)
 *
 * 使い方
 *
 * - RollingHash rh:= ローリングハッシュを宣言
 *
 * - auto t = build(s):= 文字列sのハッシュ値を計算
 *
 * - query(t, l, r):= 文字列sの[l, r)のハッシュ値を計算
 *
 * - combine(h1, h2, h2len):= ハッシュ値h1と長さh2lenのハッシュ値h2を結合する
 *
 * - lcp(a, l1, r1, b, l2, r2):= ハッシュテーブルaの区間[l1,r1)と、ハッシュテーブルbの区間[l2,r2)の最長共通接頭辞の長さを求める
 *
 * @see https://qiita.com/keymoon/items/11fac5627672a6d6a9f6
 * @see https://ei1333.github.io/library/string/rolling-hash.hpp
 * Comparison: archived ei1333/library RollingHash; Unlicense, source path unconfirmed.
 * https://competitive-verifier.github.io/zzz_ei1333_library/string/rolling-hash.hpp.html
 */
struct RollingHash {
  static const uint64_t mod = (1ull << 61ull) - 1;
  using uint128_t = __uint128_t;
  const uint64_t base;
  std::vector<uint64_t> power;
  /// @brief 加算 @param a 加数 @param b 加数 @return 和
  static inline uint64_t add(uint64_t a, uint64_t b) {
    if ((a += b) >= mod) a -= mod;
    return a;
  }
  /// @brief 乗算 @param a 乗数 @param b 乗数 @return 積
  static inline uint64_t mul(uint64_t a, uint64_t b) {
    uint128_t c = (uint128_t)a * b;
    return add(c >> 61, c & mod);
  }
  /// @brief 2^61-1 未満の乱数を生成する
  static inline uint64_t generate_base() {
    std::mt19937_64 mt(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<uint64_t> rand(256, RollingHash::mod - 256);
    return rand(mt);
  }
  /// @brief ハッシュテーブルのサイズを拡張する @param sz 拡張するサイズ
  inline void expand(size_t sz) {
    if (power.size() < sz + 1) {
      int pre_sz = (int)power.size();
      power.resize(sz + 1);
      for (int i = pre_sz - 1; i < (int)sz; i++) {
        power[i + 1] = mul(power[i], base);
      }
    }
  }

  explicit RollingHash(uint64_t base = generate_base()) : base(base), power{1} {}
  /// @brief  文字列sのハッシュ値を計算する @param s 文字列 @return ハッシュ値
  std::vector<uint64_t> build(const std::string &s) const {
    int sz = s.size();
    std::vector<uint64_t> hashed(sz + 1);
    for (int i = 0; i < sz; i++) {
      hashed[i + 1] = add(mul(hashed[i], base), static_cast<unsigned char>(s[i]));
    }
    return hashed;
  }
  /// @brief  ベクタsのハッシュ値を計算する @tparam T ベクタの型 @param s ベクタ @return ハッシュ値
  template <typename T>
  std::vector<uint64_t> build(const std::vector<T> &s) const {
    int sz = s.size();
    std::vector<uint64_t> hashed(sz + 1);
    for (int i = 0; i < sz; i++) {
      hashed[i + 1] = add(mul(hashed[i], base), s[i]);
    }
    return hashed;
  }
  /// @brief  文字列sの[l, r)のハッシュ値を計算する @param s 文字列 @param l 左端 @param r 右端 @return ハッシュ値
  uint64_t query(const std::vector<uint64_t> &s, int l, int r) {
    expand(r - l);
    return add(s[r], mod - mul(s[l], power[r - l]));
  }
  /// @brief  ハッシュ値h1とハッシュ値h2を結合する @param h1 ハッシュ値1 @param h2 ハッシュ値2 @param h2len ハッシュ値2の長さ @return 結合されたハッシュ値
  uint64_t combine(uint64_t h1, uint64_t h2, size_t h2len) {
    expand(h2len);
    return add(mul(h1, power[h2len]), h2);
  }
  /// @brief ハッシュテーブルaの区間[l1,r1)と、ハッシュテーブルbの区間[l2,r2)の最長共通接頭辞の長さを求める @param a ハッシュテーブルa @param l1 左端 @param r1 右端 @param b ハッシュテーブルb @param l2 左端 @param r2 右端 @return 最長共通接頭辞の長さ
  int lcp(const std::vector<uint64_t> &a, int l1, int r1, const std::vector<uint64_t> &b, int l2, int r2) {
    int len = std::min(r1 - l1, r2 - l2);
    int low = 0, high = len + 1;
    while (high - low > 1) {
      int mid = (low + high) / 2;
      if (query(a, l1, l1 + mid) == query(b, l2, l2 + mid))
        low = mid;
      else
        high = mid;
    }
    return low;
  }
};
