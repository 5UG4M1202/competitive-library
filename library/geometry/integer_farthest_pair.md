---
title: 整数点集合の最遠点対
documentation_of: ./integer_farthest_pair.hpp
---

# 整数点集合の最遠点対

`cp::geometry::farthest_pair(points)` は元の入力に対する異なる2つの添字 `first`, `second` と距離の二乗 `distance2` を返します。戻り値は `std::optional<FarthestPair>` で、入力が2点未満なら `std::nullopt` です。重複点を許し、全点が同じでも異なる添字を返します。最大距離の組が複数ある場合、どれを返すかは規定しません。

```cpp
#include "library/geometry/integer_farthest_pair.hpp"
#include <cassert>
#include <vector>
using namespace cp::geometry;

int main() {
  std::vector<IntegerPoint> points = { {0, 0}, {3, 0}, {0, 4} };
  auto result = farthest_pair(points);
  assert(result && result->first != result->second && result->distance2 == 25);
}
```

共通の `squared_distance(a,b)` を使い、座標差を広い型にしてから距離の二乗を計算します。この関数は [整数点と偏角](integer_point.md) にあり、このヘッダーからも引き続き使えます。平方根は取りません。戻り値は `IntegerWide`（GCC/Clang の `__int128_t`）です。

入力のコピーと凸包の構築を含め、`O(n log n)` 時間、`O(n)` 追加メモリです。凸包構築後の走査は `O(h)`（凸包の頂点数 `h`）で、元の添字の対応付けにもソートを使います。座標差・二乗・その和と凸包の向きの各中間結果が符号付き128ビットに収まる必要があります。[整数点と偏角](integer_point.md) の範囲の説明も参照してください。オーバーフローは検出しません。実数座標には対応しません。

重複・全点一致・一直線・大きな座標や小規模なランダム入力を、全点対の素朴探索と比較します。[Library Checker: Furthest Pair](https://judge.yosupo.jp/problem/furthest_pair) でも添字と最遠距離を検証します。
