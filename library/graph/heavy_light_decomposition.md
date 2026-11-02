---
title: 重軽分解と方向付きパス集約
documentation_of: ./heavy_light_decomposition.hpp
---

# 重軽分解と方向付きパス集約

固定根の無向木のパスを連続区間に分解し、LCA、部分木の区間、頂点・辺の順序付きパス集約を提供します。文字列連結や関数合成のような非可換演算と、辺の行き・帰りで異なる値に対応します。木の辺の追加・削除や根の変更は扱いません。

## 最小例：方向別の辺を連結する

```cpp
#include "library/graph/heavy_light_decomposition.hpp"
#include "library/data_structure/monoid_segtree.hpp"
#include <cassert>
#include <string>
#include <vector>

struct Concat {
  using value_type = std::string;
  static value_type op(value_type a, value_type b) { return a + b; }
  static value_type id() { return {}; }
};

int main() {
  std::vector<std::vector<int>> g{ {1}, {0, 2}, {1} };
  cp::HeavyLightDecomposition hld(g, 0);
  using B = cp::BidirectionalMonoid<Concat>;
  cp::Segtree<B> seg(3);
  auto paths = cp::make_hld_path_fold<Concat>(hld, seg);
  paths.set_edge(0, 1, "A", "a"); // 0→1, 1→0
  paths.set_edge(2, 1, "b", "B"); // 2→1, 1→2
  assert(paths.edges(0, 2) == "AB");
  assert(paths.edges(2, 0) == "ba");
  assert(paths.edges(1, 1).empty());
}
```

`set_edge(u,v,uv,vu)` は引数順で `uv` が `u→v`、`vu` が `v→u` の値です。内部では親から子へ向かう値と逆向きの値を子の位置に保存します。`u,v` は実在する親子辺でなければなりません。無向重みなら両方向に同じ値を渡せます。既存 `Graph<T>` は両隣接辺に同じ `cost` を格納するため、異なる方向別の値は明示的に指定してください。

検証ページのコード欄では「提出用の利用例」を選ぶと、上の例と必要な自作ヘッダーをまとめてコピーできます。ACL の `#include` は残るため、提出先に ACL が必要です。

## 構築と区間分解

`cp::HeavyLightDecomposition hld(g, root)` の入力は、各 `g[v]` を反復でき、その辺を `int` に変換すると隣接頂点を得られるグラフです。`std::vector<std::vector<int>>` と既存 `Graph<T>` の両方に対応します。`n >= 1`、`0 <= root < n`、辺が両方向に登録された連結な単純無向木が前提です。不正な木に対する検証は保証しません。グラフは変更・保持しないため、構築後に破棄できます。構築は反復的で、深い木でも再帰スタックを使いません。

| API | 結果 |
| --- | --- |
| `size()`, `root()` | 頂点数、固定根 |
| `parent(v)`, `depth(v)` | 親と深さ。根の親は `-1` |
| `index(v)`, `vertex(i)` | 頂点と HLD 配列位置の相互変換 |
| `subtree_size(v)` | `v` を含む部分木の頂点数 |
| `lca(u,v)` | 固定根での最小共通祖先 |
| `subtree_vertices(v)` | `v` を含む部分木の頂点範囲 `[l,r)` |
| `subtree_edges(v)` | `v` の親辺を除く部分木内の辺範囲 `[l,r)` |
| `path_vertices(u,v)` | `u` と `v` を含む頂点パスの方向付き区間列 |
| `path_edges(u,v)` | `u→v` の辺パスの方向付き区間列。`u == v` なら空 |
| `for_each_path_vertices(u,v,visit)` | 頂点パスの各区間を訪問順に `visit(HldSegment)` へ渡す |
| `for_each_path_edges(u,v,visit)` | 辺パスの各区間を訪問順に `visit(HldSegment)` へ渡す |

`HldSegment` の `{l,r,reversed}` は、`reversed == false` なら `l,l+1,...,r-1`、`true` なら `r-1,...,l` の順に訪問します。区間列そのものも `u→v` の訪問順です。辺では配列位置を子側頂点と見なし、根の位置を含めません。部分木は構築時の固定根によって定まります。

## 頂点と順逆集約

`BidirectionalMonoid<M>` は `M::value_type`、`M::op(a,b)`、`M::id()` を必要とします。`op(a,b)` を「列 `a` に列 `b` を続ける」結合として定義し、結合律と単位元を満たしてください。関数合成の場合、実際の合成順が期待する訪問順になるよう `op` を定めます。区間を反対向きに読む場合の集約も保持するため、逆元は必要ありません。

`B::vertex(x)` は `{x,x}`、`B::edge(down,up)` は `{down,up}`、`B::id()` は `{M::id(),M::id()}` です。`HldPathFold` は HLD とセグメント木を**参照**し所有しません。一時 HLD の直接指定はコンパイル時に拒否しますが、名前付き HLD やセグメント木についても、アダプターより長生きさせる必要があります。セグメント木は少なくとも `hld.size()` 要素を持ち、`set` と `prod` を提供してください。頂点値と辺値は別のセグメント木に保存します。

頂点用には `paths.set_vertex(v,x)` と `paths.vertices(u,v)`、辺用には `paths.set_edge(...)` と `paths.edges(u,v)` を使います。頂点パスは両端と LCA を含み、`vertices(v,v)` はその頂点の値です。これは [EulerTour](euler_tour.md) の `query_vert` が LCA を除くことと異なります。初期値を用意する場合は HLD 順に `B::vertex` または `B::edge` を配置し、辺の根位置には `B::id()` を入れます。

## 区間更新

HLD の `path_vertices` / `path_edges` の区間に対して、ACL の `lazy_segtree` などの `apply(l,r,f)` を呼び出せます。部分木も上の半開区間として更新できます。順序と向きに影響しない頂点への区間加算はこの方法で使えます。

非可換な集約や方向別の辺値に遅延作用を組み合わせる場合は、順逆の両集約と作用の合成が分配法則・単位元を満たすことが必要です。特に、`u→v` に沿う辺の値だけを更新する場合は `reversed` に応じて更新する成分が変わります。これらを自動的に処理する遅延アダプターは提供していません。

## 計算量と検証

構築は `O(n)` 時間・メモリ、LCA とパスの分解は `O(log n)`、部分木範囲取得は `O(1)` です。`op` が `O(1)` でセグメント木の区間取得が `O(log n)` のとき、`HldPathFold` のパス集約は `O(log² n)`、点更新は `O(log n)` です。文字列連結等では値のコピー・演算にかかる時間とメモリを別途考慮してください。区間列も `O(log n)` 個の追加メモリを使います。各種インデックスと頂点数は `int` で表現可能である必要があります。

`for_each_path_*` と `HldPathFold` は、HLD の区間列をヒープ確保せず、`int` のビット数に基づく固定長の一時領域に保持します。利用者の `visit`、モノイド値・演算、セグメント木自体が確保を行う場合は別です。`path_*` は従来どおり `std::vector` を返し、空でない場合に区間列用の領域を1回確保します。

ローカルテストは、1頂点・星形・ランダムな木の全頂点対で、BFS で求めた経路と方向別の文字列連結を比較します。親・深さ・部分木、辺の端点を逆順に渡す更新、20万頂点の鎖も検証します。別のテストでは頂点パス・部分木の区間加算を配列モデルと比較し、固定サイズの値を使って経路走査の確保回数も検査します。検証ページの「Verified with」から確認できます。

[Library Checker: Vertex Set Path Composite](https://judge.yosupo.jp/problem/vertex_set_path_composite) のテストは、頂点の一次関数の更新と、順序が重要なパス合成を検証します。方向ごとに値が異なる辺や部分木更新を確認するものではありません。

[Vertex Add Subtree Sum](https://judge.yosupo.jp/problem/vertex_add_subtree_sum) では、固定根の部分木範囲と頂点加算を検証します。

重軽分解の一般的な説明と正逆集約の考え方は [AtCoder の解説](https://info.atcoder.jp/entry/algorithm_lectures/heavy_light_decomposition)を参照してください。設計の経緯と未対応の方向依存の遅延更新はリポジトリの `docs/hld-design.md` に記録しています。
