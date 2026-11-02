---
title: 整数点集合の最近点対
documentation_of: ./integer_closest_pair.hpp
---

# 整数点集合の最近点対

`cp::geometry::closest_pair(points)` は、元の入力に対する異なる2つの添字 `first`, `second` と距離の二乗 `distance2` を返します。戻り値は `std::optional<ClosestPair>` で、入力が2点未満なら `std::nullopt` です。同じ座標の点が複数あるときは、異なる添字を持つ距離0の組を返します。最小距離の組が複数ある場合、どれを返すかは規定しません。

```cpp
#include "library/geometry/integer_closest_pair.hpp"
#include <cassert>
#include <vector>
using namespace cp::geometry;

int main() {
  std::vector<IntegerPoint> points = { {0, 0}, {5, 0}, {1, 1} };
  auto result = closest_pair(points);
  assert(result && result->first != result->second && result->distance2 == 2);
}
```

入力を変更せず、座標の並べ替えと分割統治を使って **`O(n log n)` 時間、`O(n)` 追加メモリ**で計算します。重複座標は先に検出します。返却値の添字は、並べ替え後ではなく元の入力の位置です。

整数座標の差・二乗・和の各中間結果が符号付き128ビットに収まる必要があり、オーバーフローは検出しません。距離計算は共通の `squared_distance(a,b)` を使用し、座標の差を広い型にしてから計算します。戻り値は GCC/Clang の `__int128_t` で、平方根は取りません。入力座標の絶対値がすべて `10^18` 以下なら、2点間の距離の二乗とその途中計算は収まります。その他の型・出力上の注意は [整数点と偏角](integer_point.md) を参照してください。

空・1点・重複・同一直線・大きな座標の境界、ランダムな小入力を全点対探索と比較し、10万点の縦一直線でも検証します。[Library Checker: Closest Pair](https://judge.yosupo.jp/problem/closest_pair) では返却された元の添字を公式チェッカーで検証します。最大距離を求める場合は [整数点集合の最遠点対](integer_farthest_pair.md) を参照してください。実数座標には対応しません。
