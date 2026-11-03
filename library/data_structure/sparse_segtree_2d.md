---
title: 座標登録型二次元セグメント木
documentation_of: ./sparse_segtree_2d.hpp
---

# 座標登録型二次元セグメント木

`cp::SparseSegtree2D<Coord, M>` は、構築前に更新候補の点を登録し、その後の一点上書きと矩形集約を行います。各 x 区間に登録された y 座標を圧縮し、外部依存の AtCoder Library `atcoder::segtree` で管理します。演算 `M::op` は結合的かつ可換、`M::id()` は単位元である必要があります。逆元は不要です。

コード欄の「提出用の利用例」は、ライブラリ本体・下の `Sum`・`main()` をまとめてコピーできます。ACL は外部のままなので提出先に ACL が必要です。`main()` は問題の入出力に合わせて変更してください。Monoid を自分で作る手順は [Monoid 定義ガイド](monoid_segtree.md) を参照してください。

```cpp
#include "library/data_structure/sparse_segtree_2d.hpp"
#include <cassert>
#include <utility>
#include <vector>
struct Sum {
  using value_type = long long;
  static value_type id() { return 0; }
  static value_type op(value_type a, value_type b) { return a + b; }
};
int main() {
  std::vector<std::pair<long long, long long>> points;
  points.emplace_back(1, 3); points.emplace_back(2, 5); points.emplace_back(2, 7);
  cp::SparseSegtree2D<long long, Sum> tree(points);
  tree.set(1, 3, 4); tree.set(2, 5, 6);
  assert(tree.prod(1, 3, 3, 6) == 10);
  tree.set(1, 3, 2); // 加算ではなく上書き
  assert(tree.get(1, 3) == 2);
}
```

点 `(x,y)` の初期値は `M::id()`、同一座標を重複登録しても同じ一つのセルです。`set(x,y,v)` の点は構築時に登録済みでなければなりません。`get(x,y)` は未登録点でも単位元を返します。`prod(xl,xr,yl,yr)` は半開矩形 `[xl,xr) × [yl,yr)` を集約し、空・逆転区間は単位元を返します。範囲端点は登録されていなくても構いません。

`Coord` の比較と等価判定は整合する全順序が必要です。整数や `cp::Rational128` は使えます。浮動小数点では NaN を使わず、通常の `<` による順序で、EPS は使いません。値と演算中間値の範囲は呼び出し側で管理してください。ACL の各一次元木の要素数は `10^8` 以下です。実用上はメモリにより先に制約されます。

`M::op` や値のコピーが更新途中に例外を送出した場合、それ以前に変更した節点は復元しません。例外後の木は再構築してください。`Rational128` の計算範囲外による例外も含みます。

登録点数を `N` とすると構築は `O(N log²(N+1))`、`set` と `prod` は `O(log²(N+1))`、`get` は `O(log(N+1))`、空間は `O(N log(N+1)+1)` です。比較・演算・コピーを定数時間と仮定します。オンラインの未知の点には [動的二次元セグメント木](dynamic_segtree_2d.md)、メモリ優先や最近傍には [kd-tree](../geometry/integer_kd_tree.md) を参照してください。

空・重複・負の座標、更新と矩形の境界、和・gcd、分数座標をローカル検証しています。[Library Checker: Point Add Rectangle Sum](https://judge.yosupo.jp/problem/point_add_rectangle_sum) は追加される座標を先読みして登録し、同一点への重みを加算する使い方を公式 18 ケースで検証しました。この問題は和以外の Monoid の検証にはなりません。
