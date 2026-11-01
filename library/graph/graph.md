---
title: グラフの基本型
documentation_of: ./graph.hpp
---

# グラフの基本型

`Graph<T>` は重み型 `T` の隣接リスト、`Edge<T>` は始点・終点・重み・辺番号を保持します。[最短路](dijkstra.md)、[全域森](kruskal.md)、[EulerTour](euler_tour.md)、[二部グラフ判定](bipartite_graph.md)で利用できます。

```cpp
#include "library/graph/graph.hpp"
Graph<long long> g(3);
g.add_edge(0, 1, 4);          // 無向辺
g.add_directed_edge(1, 2, 7); // 有向辺
auto cost = g[1][0].cost;      // 4
```

頂点番号は `0..N-1` です。`add_directed_edge` は片方向、`add_edge` は両方向の隣接辺を追加します。無向辺の両方向は同じ `idx` を持ち、`es` は呼び出した辺の本数です。`operator[]` から隣接辺にアクセスできます。`Edge` の `int` への変換と出力演算子は終点 `to` を返します。辺の比較は重み `cost` によります。

`Graph()` は0頂点・辺数0、`Graph(N)` は N 頂点の空グラフです。`read(M, padding=-1, weighted=false, directed=false)` は標準入力から M 辺を追加します。既定の `padding=-1` は入力が1始まりの場合を想定します。重みを読み取る場合は `weighted=true`、有向辺なら `directed=true` を指定します。

`N >= 0`、辺の両端が有効な頂点、入力が成功することが前提です。辺数と辺番号は `int` で表現できる必要があります。範囲検査はなく、公開されている隣接リストを直接変更すると `es` や辺番号が整合しなくなることがあります。`Edge()` の各値は未初期化なので、代入せずに読み取らないでください。

構築は `O(N)`、辺の追加は償却 `O(1)`、メモリは `O(N+E)`（無向辺は2つの隣接要素）です。`cpp.json` の従来型を分離し、既定構築時に `es` が未初期化になる問題を修正しました。

[ei1333/library のグラフ型](https://github.com/ei1333/library/blob/088b4c8b49f85194099305d998e7113dc3d5dd2e/graph/graph-template.hpp)と構造が非常に近く、比較した版は [Unlicense](https://github.com/ei1333/library/blob/088b4c8b49f85194099305d998e7113dc3d5dd2e/LICENSE) です。比較版にはない辺比較・出力演算子が追加されています。実際の取り込み経緯は未確認で、[第三者通知](../../THIRD_PARTY_NOTICES.md)に記録しています。
