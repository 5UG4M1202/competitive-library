---
title: 整数点の矩形集約 kd-tree
documentation_of: ./integer_kd_tree.hpp
---

# 整数点の矩形集約 kd-tree

固定された整数2次元点の集合に値を持たせ、半開矩形内の値を可換 Monoid で集約します。同じ座標の点も入力番号ごとに別々に扱います。

```cpp
#include "library/geometry/integer_kd_tree.hpp"
#include <cassert>
#include <vector>
struct Sum {
  using value_type = long long;
  static value_type id() { return 0; }
  static value_type op(value_type a, value_type b) { return a + b; }
};
int main() {
  using namespace cp::geometry;
  std::vector<IntegerPoint> p = { {0, 0}, {2, 1}, {2, 1}};
  IntegerKdTree<Sum> tree(p, {3, 4, 5});
  assert(tree.prod(0, 3, 0, 2) == 12);
  tree.set(1, 10); // 入力時の番号 1 の値を上書き
  assert(tree.prod(2, 3, 1, 2) == 15);
}
```

`M::value_type`, `M::id()`, `M::op(a,b)` を指定します。`op` は結合的・可換で `id` は単位元である必要があります。これらの性質は自動検査されません。最小値、最大値、gcd や最小値と個数などにも利用でき、逆元は不要です。各値・演算結果の有効範囲とオーバーフローは呼び出し側の責任です。外部ライブラリへの依存はありません。

構築時の座標・値は同じ長さで、点数は `INT_MAX` 以下にしてください。`set(i, value)` は入力番号 `i` の値の上書きで、`i` は有効な番号が必要です。点の座標変更・新規挿入・削除、矩形内の点の列挙や最近傍検索は未対応です。点を仮登録して値を単位元にしておくことはできますが、オンラインでの座標追加ではありません。

`prod(xl, xr, yl, yr)` は `[xl,xr) × [yl,yr)` の集約を返し、空矩形または空木では単位元を返します。引数は `IntegerWide`（GCC/Clang の `__int128_t`）なので、`IntegerWide(INT64_MAX) + 1` を上端に指定すれば最大座標を含められます。`long long` のまま加算しないでください。

部分木の外接矩形を使って探索を省略し、矩形に全体が入る部分木の集約をまとめて利用します。構築は `std::nth_element` の平均計算量を前提に平均 `O(n log n)`、点更新は `O(log n)`、矩形集約は訪問数に比例し最悪 `O(n)`、保持メモリは `O(n)` です。演算と値のコピーが定数時間の場合の評価です。入力分布によって速度は変わり、最悪時の高速性は保証しません。矩形和だけならオフライン走査なども候補です。

小入力では単純な全点走査とのランダム比較、重複・空・座標端点・更新・gcd を検証します。[Library Checker: Rectangle Sum](https://judge.yosupo.jp/problem/rectangle_sum) の公式14ケースは期待出力と一致しました。ローカルの性能測定では分布と条件により実行時間に差があり、公式環境での時間内通過は確認できていません。この問題は静的な矩形和のみの検証で、更新や他の Monoid は対象外です。測定条件は [kd-tree 設計](../../docs/kd-tree-design.md) を参照してください。
