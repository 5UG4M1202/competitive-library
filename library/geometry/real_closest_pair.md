---
title: 実数点集合の最近点対
documentation_of: ./real_closest_pair.hpp
---

# 実数点集合の最近点対

`closest_pair(vector<RealPoint>)` は異なる入力番号 `first`, `second` と `long double` の二乗距離を optional で返します。2点未満は空、同座標の点は0です。距離の比較は EPS を使わず通常の浮動小数点比較です。値や選ばれる点は丸め誤差の影響を受けます。

```cpp
#include "library/geometry/real_closest_pair.hpp"
#include <cassert>
int main() {
  using namespace cp::geometry;
  auto r = closest_pair(std::vector<RealPoint>{ {0,0},{0.5L,0},{3,0}});
  assert(r && r->first != r->second && r->distance2 == 0.25L);
}
```

有限の座標と、差・二乗・和を含む有限の中間結果が必要です。実数演算の理想的な演算モデルで `O(n log n)` 時間・`O(n)` メモリですが、丸めや極端な近接値のもとでは厳密な幾何学的結果やこの計算量を保証しません。
