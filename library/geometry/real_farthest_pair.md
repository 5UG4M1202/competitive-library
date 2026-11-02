---
title: 実数点集合の最遠点対
documentation_of: ./real_farthest_pair.hpp
---

# 実数点集合の最遠点対

`farthest_pair(points, geo=Geometry2D{})` は異なる入力番号と `long double` の二乗距離を optional で返します。2点未満は空、全点が同座標なら0です。凸包構築に指定した EPS を使い、距離の比較は通常の浮動小数点比較です。近接・退化した点では、丸めと EPS により厳密な最遠点対を保証しません。

```cpp
#include "library/geometry/real_farthest_pair.hpp"
#include <cassert>
int main() {
  using namespace cp::geometry;
  auto r = farthest_pair(std::vector<RealPoint>{ {0,0},{0.5L,0},{3,0}}, Geometry2D(1e-9L));
  assert(r && r->first != r->second && r->distance2 == 9.0L);
}
```

有限の座標と全ての中間結果が必要です。理想的な実数演算モデルでは `O(n log n)` 時間、`O(n)` メモリです。EPS が作る凸包が不整合になる場合、正しさと走査の計算量を保証しません。
