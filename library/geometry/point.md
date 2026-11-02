---
title: N 次元の点
documentation_of: ./point.hpp
---

# N 次元の点

`Point<T, N>` は固定次元の座標です。2次元は `Point<T,2>{x,y}` と `p.x`, `p.y`, `p[0]`, `p[1]`、他の次元は `Point<T,3>{ {x,y,z}}` と `p[i]` が使えます。加減算と辞書順比較を持ち、EPS は保持しません。

整数2次元アルゴリズムの引数は `IntegerPoint`（`Point<std::int64_t,2>`）、実数2次元アルゴリズムの引数は `RealPoint`（`Point<long double,2>`）です。任意の `T` や次元の `Point` をそのまま凸包や KD-tree に渡せるわけではありません。`std::int64_t` と `long long` は環境によって異なる型です。

```cpp
#include "library/geometry/point.hpp"
#include <cassert>
int main() {
  using namespace cp::geometry;
  Point<int,3> a{ {1,2,3}}, b{ {4,5,6}};
  assert(dot_n<long long>(a,b) == 32);
  auto c = cross3<long long>(a,b);
  assert(c[0] == -3 && c[1] == 6 && c[2] == -3);
}
```

`dot_n<Wide>`、`squared_distance_n<Wide>`、`cross2<Wide>`、`cross3<Wide>` は計算前に座標を `Wide` に変換します。全ての積・差・部分和が `Wide` に収まる必要があります。点の加減算も `T` の範囲内が必要です。`N` 次元の基本操作は `O(N)` 時間・`O(N)` 値、2次元と3次元の外積は定数時間です。整数の有限幅と実数の丸め・オーバーフローを呼び出し側で管理します。浮動小数点では NaN を順序比較に渡せません。
