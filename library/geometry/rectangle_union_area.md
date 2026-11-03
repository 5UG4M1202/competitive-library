---
title: 整数座標の矩形和集合面積
documentation_of: ./rectangle_union_area.hpp
---

# 整数座標の矩形和集合面積

`rectangle_union_area` は整数座標の長方形を受け取り、重なりを一度だけ数えた面積を返します。y 座標の圧縮・走査線・被覆長の管理は内部で行うため、呼び出し側の座標圧縮は不要です。

```cpp
#include "library/geometry/rectangle_union_area.hpp"
#include <cassert>
#include <iostream>
#include <vector>
int main() {
  using namespace cp::geometry;
  std::vector<IntegerRectangle> rects;
  rects.push_back({0, 0, 3, 2}); // x=[0,3), y=[0,2)
  rects.push_back({2, 0, 4, 2});
  auto area = rectangle_union_area(rects);
  assert(area == 8);
  std::cout << static_cast<long long>(area) << '\n'; // この例は 64 ビットに収まる
}
```

検証ページの「提出用の利用例」は本体と `main()` を一括コピーできます。GCC/Clang の `__int128_t` と C++20 を使用し、外部ライブラリは不要です。`main()` は問題の入出力に合わせて変更してください。

各矩形は `{xl, yl, xr, yr}`、`[xl,xr) × [yl,yr)` で、`xl <= xr`, `yl <= yr` が必要です。幅または高さが 0 の矩形は無視し、空の入力は 0 です。座標型は `int64_t`、返り値は非負の符号付き `__int128_t` です。長さの差は幅広の型で計算します。面積が符号付き 128 ビットに収まらなければ `std::overflow_error`、矩形数が `int` の最大値を超える場合は `std::length_error` です。整数の全域を使った巨大な正方形の面積は `__int128_t` に収まるとは限りません。

標準の `std::cout` は `__int128_t` をそのまま出力できません。例のように `long long` へ変換するのは、問題の制約から面積がその範囲に収まる場合だけです。より大きな面積を出力する場合は、128 ビット整数の十進出力処理を別途用意します。

実数座標には対応しません。面積だけを返し、覆われた領域や各点の被覆数は返しません。二次元いもすは座標圧縮後の境界の種類数を `X,Y` として `O(XY)` の表が必要ですが、セルごとの被覆数も得られます。本関数の走査線は面積を求めることに特化し、`O(N)` メモリで処理します。

矩形数を `N` とすると時間 `O(N log(N+1))`、追加メモリ `O(N)` です。空・重複・接触・ゼロ面積・符号付き整数端・オーバーフローに加え、小さいランダムな矩形を格子上の全走査と比較します。[Library Checker: Area of Union of Rectangles](https://judge.yosupo.jp/problem/area_of_union_of_rectangles) の公式テストも使います。

公式 21 ケースを GCC 11.4 `-O2`・WSL2 で実行した参考値は、最も時間のかかったケースが約 1.8 秒、最大メモリが約 144 MB でした。ケースには最大 50 万矩形が含まれます。計測値は環境・入力に依存し、提出先での時間やメモリを保証しません。
