---
title: 実数座標の凸包
documentation_of: ./real_convex_hull.hpp
---

# 実数座標の凸包

`convex_hull(points, keep_collinear=false, geo=Geometry2D{})` は `long double` の点の凸包を反時計回りに返します。重複は EPS ではなく座標の通常の等値比較で除去します。全点が同一直線と判定された場合、`keep_collinear` が false なら端点、true なら異なる点を辞書順に返します。開始点を末尾に繰り返しません。

```cpp
#include "library/geometry/real_convex_hull.hpp"
#include <cassert>
int main() {
  using namespace cp::geometry;
  Geometry2D geo(1e-9L);
  auto h = convex_hull(std::vector<RealPoint>{ {0,0},{1,0},{0,1},{0.25L,0.25L}}, false, geo);
  assert(h.size() == 3);
}
```

有限座標と有限の中間結果が必要です。EPS に近い向き判定が整合しない場合、厳密な凸包や境界の順序は保証されません。ソート比較には EPS を使いません。`O(n log n)` 時間・`O(n)` メモリです。包含判定用には `keep_collinear=false` を使います。
