# 未分離スニペットの整理

`cpp.json` には保守対象の6エントリのほかに16エントリがある。ここでの分類は作業候補であり、来歴・ライセンスや動作の確認結果を意味しない。特に `cpptemplate` には複数の実装が含まれるため、エントリ数と分離対象の実装数は一致しない。

| prefix | 現状と扱いの候補 |
| --- | --- |
| `cpptemplate` | 入出力・大小更新・PBDS alias は snippet 内に保持。型・マクロ・外部依存を `template-usage.md` に記録し、全体展開を検査する。 |
| `paizatemplate` | 出力は snippet 内に保持。組み込み dsu の比較元・CC0 と、全体展開・マクロを検査する。 |
| `@mos` | 入力・クエリ処理を含む使用例。右端変換とゼロ件を修正し、独立 API `Mo` と別々にテストする。 |
| `@segtree_lazy`, `@segtree_normal`, `@segtree_dynamic` | セグメント木の設定・使用例。依存実装、演算の整合性、初期化方法を確認する。 |
| `pairhash` | 単純なハッシュ補助として snippet 内で保守。未完成の宣言は名前付き placeholder に修正済み。比較元候補と未確認の来歴を第三者通知に記録。 |
| `@meguru`, `nextperm`, `nextcomb` | 変数や他の実装に依存する操作例。二分探索のオーバーフローを修正し、依存と前提を `snippet-examples.md` に記載。組合せ本体はコピー元と条件を別途記録。 |
| `dxdy4`, `dxdy8`, `@nxny` | 方向配列と走査例。座標規約、`GRID` との違い、変数の再定義を `snippet-examples.md` に記載。 |
| `output`, `for`, `if` | エディタの短い入力補助。通常はライブラリヘッダーではなく snippet として維持する。 |

## `cpptemplate` の調査単位

少なくとも入出力、素数、グラフ、重み付き Union-Find、グリッド、二次元累積和、座標圧縮、RollingHash、セグメント木、二部グラフ、ダブリングなどが含まれる。記載は網羅的な API 一覧ではない。共通の型 alias、マクロ、AtCoder Library と他の実装への依存を確認し、機能単位に境界テスト・比較テスト・文書を用意する。

RollingHash は分離済み。コメントに記載された ei1333/library の[旧版](https://competitive-verifier.github.io/zzz_ei1333_library/string/rolling-hash.hpp.html)と実装が非常に近いことを確認し、比較対象の Unlicense を記録した。実際の取得経路は未確認。符号付き char による非 ASCII バイトのハッシュを修正した。

`PrimeUtil` は利用者が生成 AI を使って作成したと確認しており、Apache-2.0 で提供する。

`to_digit` は C++20 の `countl_zero` を使う 64 ビット符号なし整数の十進桁数補助として snippet 内で保守する。具体的な来歴は未確認。

`_pow` と `mnht` は両テンプレート内で保守し、負の指数の前提と距離計算の型の制限を文書化する。`_pow` は既存 API を維持するが、グローバルの予約識別子である点に注意する。どちらも具体的な来歴は未確認。

`@mos` の汎用的な区間走査を `Mo` として分離し、左右別コールバックを用意した。既存 snippet は使用例として残し、右端の変換とゼロ件の問題を修正した。来歴は未確認。

`scan` / `in` と出力補助はテンプレート内で保守する。入れ子型が再帰的に解決されずコンパイルに失敗する問題と、`vector<bool>` の入力失敗時の未初期化値は修正済み。両テンプレートのマクロと全体展開を検査し、`template-usage.md` に依存と前提を記載した。具体的な来歴は未確認。

`chmax` / `chmin` と PBDS の単純な alias `ordered_set_less` はテンプレート内で保守する。大小更新は AtCoder の教材に同じ実装があるが実際の来歴は未確認。順序統計付き集合は GNU libstdc++ PBDS に依存し、重複や範囲外取得の条件を記載した。

`PrimeUtil`、`rotate_matrix`、`levenstein_distance`、`CoordCompressor`、`Doubling`、`GRID`、`BipartiteGraph` は分離し、ヘッダーと template の一致を検査する。座標圧縮は再構築時の古い対応が残る問題、ダブリングは最大歩数0と大きな最大歩数での未定義動作、GRID は再読み込み時の幅変更と無効な始点の問題を修正済み。二部グラフは継承する dsu を外部から変更すると判定結果が更新されない制約を記載した。PrimeUtil 以外の具体的な来歴は未確認で、所有者の著作物性のある人による寄与に Apache-2.0 を適用する。`next_combination` の旧版は [mdstoy の記事](https://qiita.com/mdstoy/items/39104b686540c5f1dc6c)からのコピーと確認された。両テンプレートの現行版は、別チャットで仕様のみから生成されたと報告された実装に置き換えた。旧版は履歴・過去の Release に残る。`CumulativeSum2D` も分離した。比較した [ei1333/library](https://github.com/ei1333/library/blob/088b4c8b49f85194099305d998e7113dc3d5dd2e/dp/cumulative-sum-2d.hpp) は Unlicense で、添字の向きを除いた構造が似ているが、類似だけで元の利用経緯を断定しない。

`Graph`・`Edge` と `dijkstra`・`kruskal` を分離した。Graph は [ei1333/library のグラフ型](https://github.com/ei1333/library/blob/088b4c8b49f85194099305d998e7113dc3d5dd2e/graph/graph-template.hpp)と非常に近く、比較版の Unlicense とローカルの追加部分を記録する。ダイクストラは距離加算のオーバーフローを修正した。

`paizatemplate` に残っていた同じグラフ型・アルゴリズムも修正を同期した。同梱 `dsu` は ACL v1.5.1 の実装と一致し、比較版の CC0 と取得経路未確認を記録する。paiza では外部 ACL を要しない。

`k_shortest_path` も分離した。ei1333/library の比較版と非常に近いが実際の来歴は未確認。`k <= 0` と距離加算のオーバーフローを修正した。

`dual_segtree` を分離した。ACL の lazy_segtree の区間更新処理と非常に近いが、実際の来歴は未確認。補助関数の名前の衝突を避け、非可換な作用を検査する。

両テンプレートの `rle` を分離し、空文字列・空配列に対する末尾参照を修正した。具体的な来歴は未確認。

両テンプレートの簡易 `is_prime`・`prime_factor`・`sieve_of_era`・`divisor` と符号付き除算を独立ヘッダーに分離した。1 以下の素数判定、0 の素因数分解・ふるい、ループや加算のオーバーフロー、負の除数を修正。旧 `out_div` は DNEK の記事の実装と一致するが、取得経路と再利用条件は未確認。

## 次の調査で注意する点

- `@segtree_normal` と `@segtree_lazy` の `seg()` が関数宣言になる問題は型 alias と `main` 内の構築例に変更した。遅延版の和と加算の不整合も、要素数を保持する区間加算・区間和へ修正した。
- `@segtree_dynamic` は `cpptemplate` 内の `dynamic_lazy_segtree` に依存し、初期状態を全要素 1 にしている。サイズは `ll` に拡張し、番兵値との衝突を防ぐため作用を `optional<ll>` とした。3例の用途、互換性の変更とテストは [`segtree-snippets.md`](segtree-snippets.md) に記載する。
- `dynamic_lazy_segtree` は未更新かつ初期値が単位元以外のとき、`max_right` / `min_left` が根がないという理由で即座に範囲端を返していた。初期値が全て 1 の `[0, 5)` で区間和が 2 以下の境界を求めると、期待値 `2, 3` に対して `5, 0` となることを再現し修正した。独立ヘッダーと snippet の一致、境界、更新を含むランダム比較を検査する。
- この動的木は境界探索でもノードを確保する。分離時に所有権と解放を実装し、浅いコピーは禁止して移動をサポートした。利用者の記憶では [ABC435 E の解説](https://atcoder.jp/contests/abc435/editorial/14743)を読んで、実装例の書き換えではなく ACL に合わせて AI に新規生成させた可能性が高いが、確かではない。実装例との再帰の共通点だけでコピーや改変は判断できず、具体的な来歴と利用条件は未確認。

## セグメント木の参照と設計上の選択肢

利用者から、セグ木について [betrue12 の ACL Lazy Segtree チートシート](https://betrue12.hateblo.jp/entry/2020/09/23/005940)を参考にしていたとの情報があった。特に `@segtree_dynamic` の区間更新・区間和に関する定義は、記事の同じ用途の例とよく似ている。これだけで実装全体の来歴や再配布条件を確定せず、記事の記載と利用条件を別途確認する。

利用者が検討中の [maspypy/library の Dynamic_SegTree](https://maspypy.github.io/library/ds/segtree/dynamic_segtree.hpp) は Monoid 型とノードプールに依存し、永続化にも対応する点更新用の動的セグ木である。既存の区間更新を持つ `dynamic_lazy_segtree` とは機能が異なる。参照は設計調査として記録し、現時点ではコードを取り込まない。

Monoid 型を採用しても ACL の置換は必須ではない。`atcoder::segtree<typename M::value_type, M::op, M::id>` のように静的メンバーを引数に渡せることを ACL でコンパイル・実行確認した。遅延版では値の演算に加えて作用の型・適用・合成・恒等作用が必要で、ACL の関数シグネチャと合成順序に合わせる。定義の共通化とデータ構造本体の選択は分けて検討し、既存 snippet を一括で書き換えない。

## 進め方

`UFstruct` は利用者が ACL の `atcoder::dsu` を使用しており不要とのため、リポジトリの snippet 集から削除した。別の実装である重み付き Union-Find は、利用者が提示した NyaanNyaan/library の CC0 実装との対応を確認し、[`union_find_with_potential.hpp`](../library/data_structure/union_find_with_potential.hpp) に分離した。

各候補について、実装の出典と適用ライセンス、現在の API、隠れた依存、既存ライブラリとの重複を先に記録する。確認できない権利関係は未確認のまま残し、一括で Apache-2.0 としない。既存の [`maintenance.md`](maintenance.md) の抽出・検証方針に従い、互換性と発見した不具合を区別する。エディタ用ひな形は無理にヘッダーにしない。
