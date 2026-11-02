---
title: 整数凸包への点の包含判定
documentation_of: ./integer_convex_contains.hpp
---

# 整数凸包への点の包含判定

`cp::geometry::convex_contains(hull, p)` は点 `p` が凸包の `outside`（外部）、`boundary`（境界上）、`inside`（内部）のどこにあるかを `PointLocation` で返します。`on_segment(a,b,p)` は端点を含む線分上かを判定します。

```cpp
#include "library/geometry/integer_convex_hull.hpp"
#include "library/geometry/integer_convex_contains.hpp"
#include <cassert>
#include <vector>
using namespace cp::geometry;

int main() {
  auto hull = convex_hull(std::vector<IntegerPoint>{ {0, 0}, {4, 0}, {4, 4}, {0, 4} });
  assert(convex_contains(hull, {2, 2}) == PointLocation::inside);
  assert(convex_contains(hull, {4, 2}) == PointLocation::boundary);
  assert(convex_contains(hull, {5, 2}) == PointLocation::outside);
}
```

入力が3点以上なら、**反時計回りの厳密な凸多角形**である必要があります。隣り合う3点が一直線上にない、同一点や末尾への始点の繰り返しがない頂点列です。`convex_hull(points)` の既定値はこの条件を満たしますが、`convex_hull(points, true)` の出力は辺上の点を含み得るため、そのまま渡せません。始点は任意です。凸性・順序の検査は行いません。

検証ページの「提出用の利用例」では、上の例と両ヘッダーの依存をまとめてコピーできます。

空集合は常に外部、1点はその点のみ境界、2点は閉線分上を境界として扱います。内部はありません。1・2点の場合は始点と終点が同じでも扱えます。`on_segment` も `a == b` を許し、一致する点だけを含みます。

1回の判定は `O(log n)` 時間・`O(1)` 追加メモリ、線分判定は `O(1)` です。整数座標・差・積と向きの中間結果は [整数点と偏角](integer_point.md) の条件が必要で、`__int128_t` を使います。小規模なランダム点集合で凸包を作り、始点を変えたうえで各辺を見る素朴判定と比較します。一般の非凸多角形や実数座標には対応しません。
