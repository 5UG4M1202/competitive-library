---
title: 実数点と偏角
documentation_of: ./real_point.hpp
---

# 実数点と偏角

`RealPoint` は `Point<long double,2>` です。入力座標と比較対象は有限値、距離・外積など全ての中間結果は有限である必要があります。`long double` の精度は環境に依存し、丸めは残ります。

```cpp
#include "library/geometry/real_point.hpp"
#include <cassert>
int main() {
  using namespace cp::geometry;
  Geometry2D geo(1e-9L); // 引数省略時は 1e-10L
  assert(geo.orientation({0,0},{2,0},{1,1e-10L}) == 0);
}
```

`Geometry2D(eps)` は有限の非負値を受け、`orientation(a,b,c)` は有向直線から点までの符号付き距離を EPS と比較し -1/0/1 を返します。`a == b` は 0 です。`orientation(a,b,c)` の自由関数は既定 EPS を使います。EPS は長さの絶対値で、どんな座標スケールにも適した誤差保証ではありません。`same_direction` / `opposite_direction` は任意の第3引数にコンテキストを指定でき、省略時は既定値を使います。ゼロベクトルは false です。

`dot`, `cross`, `norm2`, `squared_distance` は `long double` の近似値です。`RealArgumentLess` は `atan2` から求めたキーを EPS なしで比較し、概ね `(-π,π]` の反時計回りです。負の x 軸は +π、ゼロは角度0、計算上同じ角度は同値です。端点付近では丸めた角度が -π になることもあります。ごく近い異なる方向の順序は丸めに左右され、真の角度順や厳密な同一直線判定を保証しません。各演算は定数時間・メモリです。整数の厳密な演算が可能な入力は整数版を検討してください。
