---
title: EulerTour
documentation_of: ./euler_tour.hpp
---

# EulerTour

根を指定した重み付き無向木を Euler tour で展開し、LCA、辺・頂点のパス和、部分木和と重み更新を提供します。LCA だけの部品ではありません。

## 使う場面

根付き木に対して LCA、パス上の辺・頂点重み和、部分木重み和、点更新と部分木一括加算を同じ構造で扱うときに使います。LCA だけが必要な場合や、更新を行わない場合にはより小さな専用実装を選ぶ方が簡単です。

## 最小利用例

このヘッダーは `Graph<T>` を同梱していません。別途分離した [グラフの基本型](graph.md) を使えます。リポジトリルートと AtCoder Library を include path に含め、C++17 以上でコンパイルしてください。

生成された検証ページの `bundled` には、この例で使う `Graph<T>` も展開されています。ACL の `#include` は残るため、提出先でも ACL が必要です。`default` はヘッダーそのままで、別途 `Graph<T>` が必要です。独自の互換グラフ型を使う場合は `default` を選んでください。

コード欄の「提出用の利用例」では、`Graph<T>`、`EulerTour` と下の例の `main()` を一括コピーできます。問題に合わせて `main()` を変更してください。

```cpp
#include <iostream>
#include <vector>
#include "library/graph/graph.hpp"
#include "library/graph/euler_tour.hpp"

int main() {
  Graph<long long> graph(4);
  graph.add_edge(0, 1, 2);
  graph.add_edge(0, 2, 3);
  graph.add_edge(1, 3, 5);

  std::vector<long long> vertex_weight = {10, 20, 30, 40};
  EulerTour<long long, long long> tour(graph, 0, vertex_weight);

  std::cout << tour.lca(3, 2) << ' '
            << tour.query_edge(3, 2) << ' '
            << tour.query_vert(3, 2) << '\n';
  std::cout << tour.query_subtree_vert(1) << '\n';

  // パス用・部分木用の両セグ木へ反映するため mode = 2 を指定する。
  tour.add_cost_subtree_vert(1, 10, 2);
  std::cout << tour.query_subtree_vert(1) << ' '
            << tour.query_vert(3, 2) << '\n';
}
```

出力:

```text
0 10 90
60
80 110
```

この例で使う種類の LCA・パス和・部分木和・`mode = 2` の更新は `verify/local/euler_tour_boundary.test.cpp` でも assertion 付きで検証しています。verifier のページでは「Verified with」からテストを開けます。

## 依存と入力

- C++17 以上。
- AtCoder Library の `atcoder/segtree` と `atcoder/lazysegtree`。このリポジトリには vendoring せず、外部の CC0-1.0 依存として扱います。
- 呼び出し側に `template<class T> struct Graph` があり、`size()`、`g[v]` の反復、各辺の `to` と `cost` を提供すること。
- 頂点番号は `0..n-1`、`n >= 1`。入力は連結な単純無向木で、各無向辺を両方向の隣接辺として登録します。閉路や多重辺、非連結グラフは対象外です。
- `root` は有効な頂点番号。頂点重みを省略した場合は全頂点を `1` とします。
- 頂点重みの配列を渡す場合は要素数を `n` にします。各重み・更新後の和や整数倍が使用する数値型の範囲に収まる必要があります。
- `E_T` と `V_T` は `0`、加減算、整数倍を扱える数値型を想定します。

## API と意味

| API | 意味 |
| --- | --- |
| `lca(u, v)` | 指定した根での最小共通祖先 |
| `query_edge(u, v)` | 単純パス上の辺重みの総和 |
| `query_vert(u, v)` | 単純パス上の頂点重みの総和から **LCA の重みを除いた値**。`u == v` なら `0` |
| `query_subtree_edge(u)` | `u` の部分木内の辺（`u` と親の辺を除く）の総和 |
| `query_subtree_vert(u)` | `u` を含む部分木の頂点重み総和 |
| `update_*`, `add_cost_*` | 辺・頂点の一点更新または加算 |
| `add_cost_subtree_*` | 部分木内の辺・頂点への一括加算 |
| `get_depth`, `get_vert`, `get_edge` | 深さまたは現在の一点重み |

更新 API の `mode` は `0` が部分木用セグ木、`1` がパス用セグ木、`2` などそれ以外が両方です。両種類のクエリを混在させる場合、更新は `mode = 2` で呼びます。根の頂点点更新と部分木一括加算も `mode = 2` で両表現へ同期できます。

区間加算を正しく集約するため、内部ノードは符号付き重み和に加えて Euler tour の入場 `+1`・退場 `-1` の係数合計を保持します。作用 `x` は集約和へ `x * 係数合計` を加えます。頂点部分木のパス用更新では、非根について部分木根自身の退場イベントも更新し、根では存在しない退場イベントを作りません。

## 計算量

- 構築: 時間・メモリともに期待 `O(n)`（再帰 DFS と辺のハッシュ表への登録）。ハッシュ表の最悪計算量は保証されません。
- 各セグ木は最初に使うときに `O(n)` で遅延構築されます。以後の各クエリ、点更新、部分木加算は、辺のハッシュ表アクセスを含め期待 `O(log n)` です。
- DFS は再帰なので、一直線の大きな木では処理系のスタック上限に達する可能性があります。

## 制約・既知の注意

- `get_edge` / `update_edge` / `add_cost_edge` の `(u, v)` は実在する木の辺でなければなりません。存在しない組を渡すと内部 map の既定挿入が起こるため、結果は保証しません。
- 内部の無向辺キーは `min(u,v) * 10^10 + max(u,v)` です。頂点番号は非負かつ `10^10` 未満で、`long long` の積がオーバーフローしない範囲に限ります。通常の競プロ制約では十分ですが、一般目的の識別子ではありません。
- `in` と `out` は公開されていますが、tour 配列の内部表現です。半開区間 `[in[u], out[u])` が部分木を表します。

## 来歴とライセンス

ユーザー申告では、`@rrrrikiOW` が GitHub Copilot の補助を使って作成したコードです。元スニペット中に、この実装の具体的な第三者ソース URL はありません。この記録は来歴の整理であり、生成断片の著作物性や第三者権利が一切ないことの保証ではありません。

人が創作した部分に対するプロジェクトの Apache-2.0 適用範囲と、外部の AtCoder Library (CC0-1.0) は [`../../LICENSE-SCOPE.md`](../../LICENSE-SCOPE.md) と [`../../THIRD_PARTY_NOTICES.md`](../../THIRD_PARTY_NOTICES.md) を参照してください。
