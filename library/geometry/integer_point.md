---
title: 整数点と偏角
documentation_of: ./integer_point.hpp
---

# 整数点と偏角

整数座標の `cp::geometry::IntegerPoint` と、内積・外積・向き・偏角比較を提供します。座標は `std::int64_t`、積を含む計算結果は GCC/Clang の拡張 `__int128_t` です。浮動小数点の点や誤差付き判定は含みません。

```cpp
#include "library/geometry/integer_point.hpp"
#include <algorithm>
#include <cassert>
#include <vector>
using namespace cp::geometry;

int main() {
  std::vector<IntegerPoint> p = { {0, 1}, {-1, 0}, {1, 0}, {0, -1} };
  std::sort(p.begin(), p.end(), ArgumentLess{});
  assert((p == std::vector<IntegerPoint>{ {0, -1}, {1, 0}, {0, 1}, {-1, 0} }));
  assert(orientation({0, 0}, {1, 0}, {0, 1}) > 0);
  assert(same_direction({1, 2}, {2, 4}));
}
```

## 操作

- 点の `+`, `-`, 単項 `-`, `==`, `<`。`<` は座標の辞書順で、偏角順ではありません。
- `dot(a,b)`, `cross(a,b)`, `norm2(a)` は内積、外積、長さの二乗。`squared_distance(a,b)` は2点間の距離の二乗です。`orientation(a,b,c)` は `a→b` に対する `c` の左右を返し、正が左、負が右、0 が一直線です。座標差も乗算前に広い型で計算します。
- `same_direction(a,b)`, `opposite_direction(a,b)` は非ゼロベクトル同士が同方向・逆方向かを判定します。いずれかがゼロなら両方とも `false` です。
- `ArgumentLess{}` は `(-π, π]` の偏角順に反時計回りで比較します。負の x 軸は末尾で、ゼロベクトルは正の x 軸と同じ角度 0 に置きます。同じ偏角のベクトルは長さに関係なく比較上同値です。番号や重みを持つ要素は、比較対象から点を取り出すラムダと組み合わせられます。元の順序を保つ必要があれば `stable_sort` を使ってください。

偏角の順序と、2点間の角度が半周未満かは別の条件です。円環走査では折り返し、同方向、反対方向と、区間端を含むかを問題に応じて扱ってください。特に `cross > 0` だけでは、異なる同方向の点を半周内から取りこぼします。

## 利用例：半周に入る点数

[ABC481 E](https://atcoder.jp/contests/abc481/tasks/abc481_e) は、各点が非ゼロで、異なる2点と原点が一直線上にないと保証します。この条件の下では、偏角順の各左端について半周未満の範囲を尺取りできます。次は点列を受け取る関数部分です。

```cpp
int max_open_semicircle(std::vector<IntegerPoint> p) {
  const int n = static_cast<int>(p.size());
  if (n == 0) return 0;
  std::sort(p.begin(), p.end(), ArgumentLess{});
  int best = 0, right = 0;
  for (int left = 0; left < n; ++left) {
    right = std::max(right, left + 1);
    while (right < left + n && cross(p[left], p[right % n]) > 0) ++right;
    best = std::max(best, right - left);
  }
  return best;
}
```

この関数に入力点を渡して得られる個数が、同問題の答えです。一般入力に対する汎用の半平面 API ではありません。同方向の点を許す場合は、それらをまとめるなどして境界と尺取りの不変条件を改めて定義してください。

## 前提と検証

座標の加減算・単項マイナスは、その結果が `int64_t` に収まることが必要です。内積・外積・距離・向きの式では、**差・積・和差の各中間結果も**符号付き128ビットに収まる必要があります。入力座標の絶対値がすべて `10^18` 以下なら、記載した点の積・距離・向きの演算は収まります。より広い範囲や演算を重ねた結果は個別に確認してください。オーバーフロー検出は行いません。`__int128_t` は標準 C++ ではなく、MSVC では利用できません。

標準の `std::ostream` は `__int128_t` の直接出力を提供しません。値を出力する場合は、出力範囲を確認して対応する型に変換するか、128ビット用の出力処理を用意してください。

各演算と比較は `O(1)`、偏角ソートは `O(n log n)` です。小さい格子点で比較の順序条件と独立した角度計算を検査し、境界・大きな座標を確認します。[Library Checker: Sort Points by Argument](https://judge.yosupo.jp/problem/sort_points_by_argument) のケースでも検証します。

凸包は [整数凸包](integer_convex_hull.md) を参照してください。
