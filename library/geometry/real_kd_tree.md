---
title: 実数座標の矩形集約 kd-tree
documentation_of: ./real_kd_tree.hpp
---

# 実数座標の矩形集約 kd-tree

固定された `long double` の2次元座標に値を持たせ、半開矩形 `[xl,xr) × [yl,yr)` 内を可換 Monoid で集約します。同じ座標の点は入力番号ごとに別の点です。

```cpp
#include "library/geometry/real_kd_tree.hpp"
#include <cassert>
#include <vector>
struct Sum {
  using value_type = long long;
  static value_type id() { return 0; }
  static value_type op(value_type a, value_type b) { return a + b; }
};
int main() {
  using namespace cp::geometry;
  std::vector<RealPoint> points = { {0.25, 1.5}, {1.25, 1.5}, {1.25, 1.5}};
  RealKdTree<Sum> tree(points, {3, 4, 5});
  assert(tree.prod(0.0, 1.0, 1.0, 2.0) == 3);
  tree.set(1, 10);
  assert(tree.prod(1.0, 2.0, 1.0, 2.0) == 15);
  auto nearest = tree.nearest({0.25L, 1.5L});
  assert(nearest && nearest->index == 0 && nearest->distance2 == 0);
  std::vector<std::size_t> ids;
  tree.for_each_in_rect(1, 2, 1, 2, [&](std::size_t id) { ids.push_back(id); });
  assert(ids.size() == 2);
}
```

点の座標は有限値のみです。クエリ端点には無限大を使えますが、NaN は点・端点ともに禁止です。座標と端点は格納された `long double` の値を通常の `<` で厳密に比較し、EPS による同値判定はしません。`-0.0` と `+0.0` は同じ座標と比較されます。十進数や計算結果は `long double` に丸められるため、数学的に同じ値を異なる計算で得ても、半開区間の辺に含まれるかが異なることがあります。必要な許容誤差は、問題の意味に合わせて呼び出し側で端点を決めてください。固定小数点で正確に扱える問題は [整数座標版](integer_kd_tree.md) も検討してください。

`M::value_type`, `M::id()`, `M::op(a,b)` は結合的・可換な演算と単位元を定義します。演算値は座標と独立した型にでき、例では整数和です。浮動小数点の和は丸めにより一般には結合則を厳密に満たさず、集約順や木の形により結果が変わり得ます。数値の範囲と誤差は呼び出し側で管理してください。

点と値の個数は等しく、点数は `INT_MAX` 以下が必要です。`set(i, value)` は入力番号による上書きで、有効な番号が必要です。コンストラクターは値と座標を木に保持するため、入力配列の寿命に依存しません。空木、空・逆転した矩形は単位元を返します。点の新規挿入・削除・座標変更は未対応です。

`nearest(p)` は入力時の点番号 `index` と距離の二乗 `distance2` を持つ optional を返します。空集合は空、同距離なら任意の1点、重複座標も別番号です。問い合わせ点は有限であり、座標差・二乗・和などの途中結果が有限である必要があります。丸めやアンダーフローにより、数学的な最近点と異なる場合があります。`for_each_in_rect(xl,xr,yl,yr,visit)` は半開矩形内の各入力番号をコールバックに渡します。EPS は使わず、順序は未規定です。呼び出し中は同じ木を変更しないでください。

演算・値のコピーを定数時間とすると、構築は `std::nth_element` の平均計算量を前提に平均 `O(n log n)`、点の更新 `O(log n)`、矩形集約・最近傍は訪問節点数に比例し最悪 `O(n)`、列挙は最悪 `O(n)` とコールバックの実行費用、メモリ `O(n)` です。整数版とは別実装です。

最近傍・列挙を追加する前の参考値として、GCC 11.4 `-O2`・WSL2 で、20万点・20万クエリの格子上の細い縦帯は構築約76 ms、全クエリ約2,045 ms、最大 RSS 約37.7 MiBでした。新旧の計測条件と入力分布によって変動し、あらゆる入力の速度を保証しません。`tools/benchmark_integer_kd_tree.cpp` を `-DBENCH_REAL_KD_TREE` 付きでコンパイルすると小数版を測定できます。

空木、重複、符号付きゼロ、隣接する浮動小数点値、有限最大値、無限端点、更新と所有権を検査し、決定的ランダム入力を全点走査と比較します。対応する公開問題での検証はまだありません。
