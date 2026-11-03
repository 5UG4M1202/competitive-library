---
title: 実数座標の線分と直線
documentation_of: ./real_segment.hpp
---

# 実数座標の線分と直線

`RealSegment` は端点を含む `RealPoint` の線分です。`contains(segment, point, geo)` は点が線分上か、`intersects(a,b,geo)` は線分が交わるかを判定します。端点一致や点に退化した線分も扱います。`Geometry2D` の EPS は座標の単位での絶対距離、既定は `1e-10L` です。

`contains` は点から線分への最短距離が EPS 以下かで判定します。端点の外側では端点までの距離を使います。`intersects` は向きと端点の距離を組み合わせた近似判定で、二線分間距離そのものを EPS と比較する API ではありません。

検証ページの「提出用の利用例」は必要な自作コードと `main()` を一括コピーできます。外部ライブラリは不要で、C++20 を使います。`main()` は問題の入出力に合わせて変更します。

```cpp
#include "library/geometry/real_segment.hpp"
#include <cassert>
int main() {
  using namespace cp::geometry;
  RealSegment a{ {0,0}, {2,2} }, b{ {0,2}, {2,0} };
  assert(intersects(a,b));
  auto p = line_intersection(a,b);
  assert(p && p->x == 1 && p->y == 1);
}
```

`line_intersection(a,b,geo)` は端点を延長した直線同士の交点で、退化・平行・一致または EPS に照らしてほぼ平行な場合は `nullopt` です。判定は方向ベクトルのそれぞれについて、もう一方の直線からの符号付き距離が EPS 以下かを用います。`projection(line,p)` は直線への射影で、計算した方向ベクトルの長さの二乗が正である必要があります。非常に小さな非零ベクトルでも二乗がアンダーフローすれば使えません。`segment_distance(s,p)` は点と線分の最短距離を返し、計算した長さの二乗が 0 の場合は点として扱います。

座標と差・内積・外積・二乗・距離・交点など途中結果は有限である必要があります。非常に短い線分、ほぼ平行な直線、端点付近は丸めと EPS の影響を受け、正確な幾何判定やスケールに依存しない誤差は保証しません。実数 EPS の等値は推移律を保証せず、ソート比較には使わないでください。各操作は定数時間・メモリです。整数で厳密に扱える場合は [整数版](integer_segment.md) も利用できます。

小さい整数座標の全組合せを整数版と比較し、端点・退化・EPS・射影・距離を検査します。丸めの影響が大きい入力の完全性は検証されていません。
