---
title: 二部グラフ判定
documentation_of: ./bipartite_graph.hpp
---

# 二部グラフ判定

各辺の両端に異なる2色を割り当てられるかを、AtCoder Library の `dsu` を使って判定します。二部グラフの場合は各頂点の色も取得できます。ACL のヘッダーが必要です。

```cpp
#include "library/graph/bipartite_graph.hpp"
struct Edge { int from, to; };
std::vector<std::vector<Edge>> graph(3);
graph[0].push_back({0, 1});
graph[1].push_back({1, 0});
graph[1].push_back({1, 2});
graph[2].push_back({2, 1});
BipartiteGraph b(graph);
bool ok = b.is_bipartite(); // true
int color = b[0];           // ok が true のとき 0 または 1
```

`graph.size()` が頂点数 `N`、各 `graph[i]` が `from` と `to` を持つ辺の列である必要があります。頂点は `0 <= from, to < N`、`from == i` とします。辺は両端の色が異なるという制約として扱い、有向辺を1回だけ渡した場合も同じ制約を課します。自己ループは二部グラフではありません。空グラフは二部グラフです。

`operator[](i)` は `is_bipartite() == true` の場合にだけ、有効な `0 <= i < N` で使ってください。二部グラフでない場合の色の意味は保証されません。連結成分ごとに0と1の向きは任意です。初回の色取得時に彩色し、その結果を保持します。

この型は互換性のため `atcoder::dsu` を公開継承していますが、構築後に継承した `merge` 等で内部の連結関係を変更しないでください。判定結果と色のキャッシュが更新されません。辺を変更したい場合は新しいインスタンスを構築します。

頂点数 `N` と辺の入力件数 `M` について、`2*N` が ACL の `int` の範囲と利用可能メモリに収まる必要があります。構築と初回の彩色は償却 `O((N+M) α(N))`、以後の色取得は償却 `O(α(N))`、追加メモリは `O(N)` です。時間は ACL の disjoint set の操作を仮定します。

`cpptemplate` の従来実装を独立ヘッダーに分離し、標準ライブラリと ACL の名前を修飾しました。従来部分の具体的な来歴は未確認です。Apache-2.0 は rrrriki の権利を持つ著作物性のある人による寄与に適用し、他者の権利を保証するものではありません。
