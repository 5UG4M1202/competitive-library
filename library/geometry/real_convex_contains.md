---
title: 実数凸包への点の包含判定
documentation_of: ./real_convex_contains.hpp
---

# 実数凸包への点の包含判定

`convex_contains(h, p, geo=Geometry2D{})` は `RealPointLocation::outside`、`boundary`、`inside` を返します。3点以上は厳密に凸な反時計回りの多角形で、末尾の開始点の重複がないことが必要です。空・1点・線分も扱います。辺上判定に EPS を使うため、境界付近は近似結果です。

```cpp
#include "library/geometry/real_convex_contains.hpp"
#include <cassert>
int main() {
  using namespace cp::geometry;
  std::vector<RealPoint> h{ {0,0},{2,0},{0,2}};
  assert(convex_contains(h,{0.5L,0.5L},Geometry2D(1e-9L)) == RealPointLocation::inside);
}
```

辺上判定では直線からの距離に加え、各座標の範囲を EPS だけ広げて比較します。この条件は端点からのユークリッド距離が EPS 以内であることと同義ではありません。有限の座標・中間結果と上述の多角形の条件が必要です。`O(log n)` 時間・`O(1)` 追加メモリです。EPS のため多角形が適切な厳密凸として扱われない場合、結果の整合性は保証されません。
