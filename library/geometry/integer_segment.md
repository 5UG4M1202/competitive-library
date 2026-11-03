---
title: 整数座標の線分と直線
documentation_of: ./integer_segment.hpp
---

# 整数座標の線分と直線

`IntegerSegment` は `IntegerPoint` の端点 `a,b` を持ち、端点を含みます。`contains` は線分上の点、`intersection_kind` は二線分の共通部分を `none`・一点 `point`・正の長さを持つ `overlap` に分けます。端点一致・同一直線の重なり・点に退化した線分も対象です。

検証ページの「提出用の利用例」は必要な自作コードと `main()` を一括コピーできます。外部ライブラリは不要で、C++20 を使います。`main()` は問題の入出力に合わせて変更します。

```cpp
#include "library/geometry/integer_segment.hpp"
#include <cassert>
int main() {
  using namespace cp::geometry;
  IntegerSegment a{ {0,0}, {2,2} }, b{ {0,2}, {2,0} };
  assert(intersection_kind(a,b) == IntegerSegmentIntersection::point);
  auto p = line_intersection(a,b);
  assert(p && p->x == cp::Rational128(1) && p->y == cp::Rational128(1));
}
```

`line_intersection(a,b)` は端点を延長した**直線同士**の交点を分数座標 `Point<Rational128,2>` で返します。平行・一致・端点が同じため直線を定義できない場合はいずれも `nullopt` です。線分の範囲内かどうかは別に `intersection_kind` で確認します。浮動小数点が必要なら [実数座標版](real_segment.md) を参照してください。

向き、差、積、外積、交点計算のすべての中間結果は符号付き `__int128_t` に収まる必要があります。`int64_t` の全域を受けても常に安全という意味ではなく、オーバーフローは検出しません。分数計算には `Rational128` の範囲と途中計算の制約・例外が別途あります。各操作は定数回の演算、交点の分数の約分には整数の桁数に依存する互除法が必要です。GCC/Clang と C++20 が必要です。

ローカルで交差・端点・重複・退化・平行・分数交点を検査します。
