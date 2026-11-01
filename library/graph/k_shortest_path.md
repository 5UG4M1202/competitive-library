---
title: K 本の単純最短路
documentation_of: ./k_shortest_path.hpp
---

# K 本の単純最短路

非負重みの有向グラフで、始点から終点への頂点を重複しない経路を短い順に最大 `k` 本求めます。各結果は `(距離, 辺番号の列)` です。同じ距離の経路の順序は保証しません。距離だけが必要なら [ダイクストラ法](dijkstra.md) も参照してください。

```cpp
#include "library/graph/k_shortest_path.hpp"
Graph<long long> g(3);
g.add_directed_edge(0, 1, 2); // 辺番号 0
g.add_directed_edge(1, 2, 3); // 辺番号 1
g.add_directed_edge(0, 2, 7); // 辺番号 2
auto paths = k_shortest_path(g, 0, 2, 2);
// (5, {0, 1}), (7, {2})
```

`s` と `t` は異なる有効な頂点、各辺の重みは非負である必要があります。辺番号は隣接リスト内の各辺に対し `0` から辺数未満まで重複なく割り振ってください。通常は `Graph::add_directed_edge` を使います。`add_edge` は両方向で同じ番号を共有するため使えません。`k <= 0` なら空配列を返します。距離型 `T` の最大値は未到達の番兵であり、それ以上の距離は表せません。加算が型の範囲を超える候補は無視します。経路数やグラフが大きいと複数回の最短路探索と候補の保持に多くの時間とメモリを使います。

従来 snippet から分離し、`k <= 0` と距離加算のオーバーフローを修正しました。[ei1333/library の実装](https://github.com/ei1333/library/blob/088b4c8b49f85194099305d998e7113dc3d5dd2e/graph/shortest-path/k-shortest-path.hpp)と非常に近く、比較版は Unlicense です。実際の取り込み経緯は未確認です。詳細は [第三者通知](../../THIRD_PARTY_NOTICES.md) を参照してください。
