# Graph

木・グラフに対する再利用可能な部品を置きます。各ヘッダーの前提、返り値の定義、依存ライブラリ、既知の制約は同名の Markdown を先に確認してください。

| 部品 | 説明 | 状態 |
| --- | --- | --- |
| [`euler_tour.hpp`](euler_tour.hpp) | 根付き木の LCA、パス和、部分木和、重み更新 | ローカル境界・ランダム比較で検証 |

`EulerTour` は AtCoder Library の `segtree` / `lazy_segtree` と、隣接辺に `to`・`cost` を持つ既存の `Graph<E_T>` を利用します。
