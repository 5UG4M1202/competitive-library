# Graph

木・グラフに対する再利用可能な部品を置きます。各ヘッダーの前提、返り値の定義、依存ライブラリ、既知の制約は同名の Markdown を先に確認してください。

| 説明 | ヘッダー | 概要 | 状態 |
| --- | --- | --- | --- |
| [グラフの基本型](graph.md) | [graph.hpp](graph.hpp) | 頂点・辺の隣接リスト | ローカルテストと他部品との連携で検証 |
| [ダイクストラ法](dijkstra.md) | [dijkstra.hpp](dijkstra.hpp) | 非負重みの単一始点最短距離 | 境界・ランダム比較で検証 |
| [K 本の単純最短路](k_shortest_path.md) | [k_shortest_path.hpp](k_shortest_path.hpp) | 有向グラフの短い単純路 | 小規模グラフの全経路と比較 |
| [クラスカル法](kruskal.md) | [kruskal.hpp](kruskal.hpp) | 最小・最大全域森の重み | 全候補列挙で検証 |
| [二部グラフ判定](bipartite_graph.md) | [bipartite_graph.hpp](bipartite_graph.hpp) | ACL の dsu を利用した判定・彩色 | ローカル境界・ランダム比較で検証 |
| [グリッド探索](grid.md) | [grid.hpp](grid.hpp) | 壁のある格子の上下左右探索 | ローカル境界・ランダム比較で検証 |
| [ダブリング](doubling.md) | [doubling.hpp](doubling.hpp) | 一意な遷移を繰り返した先 | ローカル境界・ランダム比較で検証 |
| [`EulerTour`](euler_tour.md) | [`euler_tour.hpp`](euler_tour.hpp) | 根付き木の LCA、パス和、部分木和、重み更新 | ローカル境界・ランダム比較で検証 |

`EulerTour` は AtCoder Library の `segtree` / `lazy_segtree` と、隣接辺に `to`・`cost` を持つ既存の `Graph<E_T>` を利用します。
