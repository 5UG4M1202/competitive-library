---
title: 整数座標の凸包
documentation_of: ./integer_convex_hull.hpp
---

# 整数座標の凸包

`cp::geometry::convex_hull(points, keep_collinear = false)` は整数点の凸包を返します。入力は値渡しで、重複点は除きます。点と演算の型・オーバーフロー条件は [整数点と偏角](integer_point.md) と同じです。

```cpp
#include "library/geometry/integer_convex_hull.hpp"
#include <cassert>
#include <vector>
using namespace cp::geometry;

int main() {
  std::vector<IntegerPoint> p = { {0, 0}, {1, 0}, {2, 0}, {2, 1}, {0, 1} };
  auto corners = convex_hull(p);       // (1,0) は除外
  auto boundary = convex_hull(p, true); // 辺上の (1,0) も残す
  assert(corners.size() == 4 && boundary.size() == 5);
}
```

3頂点以上なら、返却順は反時計回りで、始点を末尾に重複して付けません。`keep_collinear=false` は辺上の頂点以外を除き、`true` は辺上の相異なる入力点を残します。全点が一直線なら前者は両端、後者は相異なる全点を辞書順で返します。空集合は空、重複除去後1点または2点ならその点列です。返り値の点順と元の入力番号は対応しません。

構築は `O(n log n)` 時間、`O(n)` 追加メモリです。入力の点が多いほど値渡しによるコピーも生じます。固定幅整数のオーバーフローを検出せず、3点の向きの各中間結果が符号付き128ビットに収まる必要があります。GCC/Clang の `__int128_t` を使います。

小さなランダム点集合で全点の包含、辺上の点、凸性、重複を独立に検査し、[Library Checker: Static Convex Hull](https://judge.yosupo.jp/problem/static_convex_hull) では頂点のみを返す既定の仕様を検証します。関連する操作は [凸包への点の包含判定](integer_convex_contains.md) と [最遠点対](integer_farthest_pair.md) を参照してください。
