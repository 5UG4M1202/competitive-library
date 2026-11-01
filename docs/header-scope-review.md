# 独立ヘッダーの保守範囲

スニペットから取り出せることだけでは、独立ヘッダーを作る理由になりません。標準・外部機能の単純な alias、競プロ用の短い補助、テンプレート専用の入出力やマクロは原則スニペットに置きます。独立した機能、既存機能にない操作やインターフェース、複数部品が共有する契約、独立して検証・保守する実益があるものはヘッダーに置きます。

## スニペットで保守するもの

| 対象 | 判断と保守方法 |
| --- | --- |
| `chmax` / `chmin` | 短い大小更新の慣用句。元のテンプレートと展開テストで保守する。 |
| `ordered_set_less` | GNU PBDS の昇順 alias で機能追加がない。依存と範囲外取得はテンプレートの使い方に記載する。機能追加やインターフェース強化時は再検討する。 |
| `scan` / `in`、`out_c` / `out_c_pairs`、宣言・走査マクロ | 提出用テンプレート固有の入出力・記法。入れ子読み取りと入力失敗の修正、展開テストは維持する。 |
| `_pow`、`mnht`、`to_digit` | 限定された型を対象とする短い数値補助。負の指数やオーバーフローなどの条件と展開テストは維持する。`to_digit` は最適化の効果を測定していないため、最適化だけを独立化の根拠にしない。 |
| `pairhash` | 特定の整数ペア向けの単純なハッシュ補助で、機能追加や衝突耐性の強化がない。出典比較・条件と展開テストは維持する。 |
| `@meguru`、`nextperm`、`nextcomb`、格子方向とエディタ入力補助 | 周囲の変数や問題固有処理を埋める例。依存と制約は `snippet-examples.md` に記載する。 |

これらは `docs/template-usage.md`、`docs/snippet-examples.md` とスニペット展開テストで見つけられます。スニペットへの整理は来歴・利用条件の確定を意味しません。

## 独立ヘッダーとして維持するもの

| ヘッダー | 維持する理由 |
| --- | --- |
| `graph/graph.hpp` | 複数のグラフアルゴリズムが共有する頂点・辺・ID の契約。 |
| `graph/dijkstra.hpp`, `graph/kruskal.hpp`, `graph/k_shortest_path.hpp` | それぞれ異なる最短路・全域森・複数単純路の独立アルゴリズム。単純な標準 API の alias ではない。 |
| `graph/bipartite_graph.hpp` | ACL dsu を利用しつつ、二部判定・彩色という別の機能を提供する。 |
| `graph/grid.hpp`, `graph/doubling.hpp`, `graph/euler_tour.hpp` | 壁を持つ格子探索、一意遷移の反復、木のパス・部分木操作という状態と異なる契約を持つ。 |
| `data_structure/dual_segtree.hpp`, `data_structure/dynamic_lazy_segtree.hpp` | 区間作用と一点取得、広い座標の動的区間作用という独立データ構造。 |
| `data_structure/monoid_segtree.hpp` | ユーザーが希望した Monoid 型・作用の定義と ACL をつなぐインターフェース。単なる型 alias ではなく、演算例と適用方法を保守する。 |
| `data_structure/cumulative_sum_2d.hpp`, `data_structure/coord_compressor.hpp` | 2 次元範囲集約や複数配列の圧縮・復元を担う状態付き部品。 |
| `data_structure/union_find_with_potential.hpp` | ACL dsu が直接提供しない重み差・整合性判定。 |
| `data_structure/interval_set.hpp`, `data_structure/lazy_reversible_rbst.hpp` | 値付き区間の操作、動的列の遅延作用・反転という独立データ構造。 |
| `data_structure/distinct_k_segments.hpp`, `data_structure/nqueen.hpp` | 特定問題に対する状態・入力条件・結果を持つアルゴリズム部品。 |
| `math/prime_util.hpp`, `math/trial_division.hpp` | 前計算して多数処理する方法と、表なしの単発素数判定・分解・約数列挙。使い分けと入力条件を保守する。 |
| `math/integer_division.hpp` | 正負の除数に対応する床・天井・ゼロから離れる丸めと、型境界を明示した契約。C++ の切り捨て除算にはない機能。 |
| `string/rolling_hash.hpp`, `string/levenstein_distance.hpp`, `string/run_length_encoding.hpp` | 部分列ハッシュ、編集距離、連続値の圧縮という異なる処理。標準の単純な別名ではなく、制約とテストを個別に保守する。 |
| `misc/mo.hpp`, `misc/rotate_matrix.hpp`, `misc/mydate.hpp` | 区間問い合わせ処理のコールバック API、長方形行列の回転、日付型と演算。それぞれの前提と利用方法は対応する文書を参照。 |

維持は正しさや来歴を保証する判定ではありません。各ヘッダーの仕様、検証と `THIRD_PARTY_NOTICES.md` を別途確認します。上記は現状の機能を基にした判断で、将来 snippet に機能追加やインターフェース強化があれば再検討します。
