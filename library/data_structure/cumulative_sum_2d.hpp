#pragma once
#include <vector>

// Closely matches ei1333/library with axis order changed; historical source unconfirmed.
// Comparison: https://github.com/ei1333/library/blob/088b4c8b49f85194099305d998e7113dc3d5dd2e/dp/cumulative-sum-2d.hpp
// Comparison source license: Unlicense. See THIRD_PARTY_NOTICES.md.
template <class T>
struct CumulativeSum2D {
  std::vector<std::vector<T> > data;

  /// @brief H x W の二次元累積和を宣言 @param H 高さ @param W 幅
  CumulativeSum2D(int H, int W) : data(H + 1, std::vector<T>(W + 1, 0)) {}

  /// @brief x, y に z を加算 @param x x座標 @param y y座標 @param z 加算する値
  void add(int x, int y, T z) {
    ++x, ++y;
    if (y >= (int)data.size() || x >= (int)data[0].size()) return;
    data[y][x] += z;
  }

  /// @brief 二次元累積和を構築
  void build() {
    for (int i = 1; i < (int)data.size(); i++) {
      for (int j = 1; j < (int)data[i].size(); j++) {
        data[i][j] += data[i][j - 1] + data[i - 1][j] - data[i - 1][j - 1];
      }
    }
  }

  /// @brief (sx, sy) から (gx, gy) までの和を計算 [sx, gx), [sy, gy)
  /// @param sx x座標の始点 @param sy y座標の始点 @param gx x座標の終点 @param gy y座標の終点 @note gxとgyは含まれない
  T query(int sx, int sy, int gx, int gy) const {
    return (data[gy][gx] - data[gy][sx] - data[sy][gx] + data[sy][sx]);
  }
};
