# Maintenance policy

## Snippet synchronization

検証ページの複数ヘッダー利用例には、依存と `main()` をまとめた「提出用の利用例」を用意します。利用例の本文とコピー用コードを同期し、完成 HTML に対する `python3 tools/test_bundled_submission.py SITE_DIR` で各ライブラリの `bundled` を単一ファイルとして構文検査します。代表的な組み合わせと利用例については実行・出力も確認します。実行には `ACL_INCLUDE_PATH` が必要です。外部ライブラリは展開しません。

独立ヘッダーを増やすこと自体は目的にしません。再利用する独自機能、強化されたインターフェース、追加機能、独立した保守価値がある場合に分離します。`chmax` / `chmin`、`out_c`、宣言マクロ、PBDS の単純な alias のような短い競プロ用補助は、原則として snippet 内で保守します。PBDS に機能追加やインターフェースの改善を行う場合は独立ヘッダーを検討します。snippet のバグ修正や検証はヘッダー化しなくても行います。

現在の各ヘッダーと snippet の判断理由は [`header-scope-review.md`](header-scope-review.md) に記録します。新たな機能や使い方が変わったときは、機械的に数や行数で判断せず、その理由を見直します。

`@mydate`、`@DistinctMax`、`@NQUEEN` はそれぞれのヘッダーから同期し、`python3 tools/check_unresolved_snippet_sync.py` で定義の一致と展開テストを確認します。ACL の include path が必要です。

保守対象として対応付けたコンポーネントでは、`library/` のヘッダーを正本とし、`cpp.json` の対応エントリを現在利用できる snippet として同期します。例えば `interval_set.hpp` と `@interval_set`、`lazy_reversible_rbst.hpp` と `@RBST`、`euler_tour.hpp` と `@EulerT`、`mydate.hpp` と `@mydate`、`distinct_k_segments.hpp` と `@DistinctMax`、`nqueen.hpp` と `@NQUEEN` が対応します。その他の対応と検査方法は以下にも記載します。

- コード、テスト、コンポーネント文書を先に更新し、対応エントリだけを同期する。
- 未整理のエントリ、`prefix`、`description`、関係のない本文と整形は変更しない。
- 出典とライセンス表示を snippet 本文にも残し、混在する権利関係を一括で再許諾しない。
- `cpp.json` は VS Code の JSONC として読み取れる状態を維持する。先頭コメントや既存の整形を JSON への再シリアライズで失わない。
- `python3 tools/check_interval_set_snippet_sync.py` で JSON 構造、対応関数、出典表示、代表テンプレートでのコンパイルと回帰を確認する。
- `python3 tools/check_rbst_snippet_sync.py` で RBST の対応定義、出典表示、代表テンプレートでのコンパイルと回帰を確認する。
- `python3 tools/check_euler_tour_snippet_sync.py` で EulerTour の全定義、来歴表示、`Graph` と AtCoder Library を含む代表テンプレートでのコンパイルと実行を確認する。実行には ACL の include path が必要。
- リポジトリ外の live VS Code snippet は別管理とし、明示された依頼なしに更新しない。

個別に来歴と依存を調べた対象だけを互換性を確認して分離し、対応付けていないエントリを一括再生成しません。分離後はヘッダーを正本として対応部分を検査します。

`cpptemplate` 内の `UnionFindWithPotential` は `library/data_structure/union_find_with_potential.hpp` に対応し、定義の比較と snippet 展開テストを `python3 tools/check_potential_union_find_snippet_sync.py` で実行します。テンプレート全体を再生成せず、この定義だけを同期します。

`cpptemplate` 内の `dynamic_lazy_segtree` は `library/data_structure/dynamic_lazy_segtree.hpp` と対応し、`python3 tools/test_dynamic_lazy_segtree_snippet.py` で定義の一致と展開テストを検査します。Apache-2.0 は利用者の著作物性のある人による寄与に適用し、生成断片の著作物性や第三者権利は保証しません。

`cpptemplate` 内の `PrimeUtil` は `library/math/prime_util.hpp` と対応し、`python3 tools/check_prime_util_snippet_sync.py` で定義の一致と展開テストを検査します。利用者が生成 AI を使って作成し、Apache-2.0 で提供します。

同様に `rotate_matrix` の2 overload は `library/misc/rotate_matrix.hpp` に対応し、`python3 tools/check_rotate_matrix_snippet_sync.py` で検査します。

`levenstein_distance` は `library/string/levenstein_distance.hpp` に対応し、`python3 tools/check_levenstein_snippet_sync.py` で検査します。ヘッダー側は複数翻訳単位で使えるよう `inline` を追加しています。

`CoordCompressor` は `library/data_structure/coord_compressor.hpp` と対応し、`python3 tools/check_coord_compressor_snippet_sync.py` で一致と再構築を検査します。再構築時に古い対応が残っていた不具合は修正済みです。

`CumulativeSum2D` は `library/data_structure/cumulative_sum_2d.hpp` と対応し、`python3 tools/check_cumulative_sum_2d_snippet_sync.py` で一致と矩形和を検査します。比較元候補の Unlicense 実装と実際の来歴は第三者通知に区別して記録します。

`Doubling` は `library/graph/doubling.hpp` と対応し、`python3 tools/check_doubling_snippet_sync.py` で一致・境界・遷移を検査します。最大歩数0と大きな歩数での従来の未定義動作を修正しています。

`GRID` は `library/graph/grid.hpp` と対応し、`python3 tools/check_grid_snippet_sync.py` で一致とグリッド操作を検査します。再読み込み時の幅の変更と、無効・壁の始点に関する従来の不具合を修正しています。

`BipartiteGraph` は `library/graph/bipartite_graph.hpp` と対応し、`python3 tools/check_bipartite_graph_snippet_sync.py` で一致と判定・彩色を検査します。ACL の include path が必要です。

`Edge`・`Graph`・`Edges`、`dijkstra`、`kruskal` はそれぞれ `library/graph/graph.hpp`、`dijkstra.hpp`、`kruskal.hpp` に対応し、`python3 tools/check_graph_algorithms_snippet_sync.py` で `cpptemplate` と `paizatemplate` の一致と展開テストを検査します。前者は ACL の include path が必要で、後者は同梱の `dsu` を用います。Graph の比較元候補と Unlicense、paiza の dsu と ACL CC0 は第三者通知に記録します。

`k_shortest_path` は `library/graph/k_shortest_path.hpp` と対応し、`python3 tools/check_k_shortest_path_snippet_sync.py` で一致と展開後の全経路比較を検査します。比較元候補の Unlicense と来歴未確認の状況を第三者通知に記録します。

`RollingHash` は `library/string/rolling_hash.hpp` と対応し、`python3 tools/check_rolling_hash_snippet_sync.py` で一致と展開後の動作を検査します。非 ASCII 文字列の符号付き char を修正しているため、従来のハッシュ値と異なる場合があります。

`dual_segtree` は `library/data_structure/dual_segtree.hpp` と対応し、`python3 tools/check_dual_segtree_snippet_sync.py` で補助関数の名前の違いを除く一致と非可換な作用のテストを実施します。

`rle` は `library/string/run_length_encoding.hpp` と対応し、`python3 tools/check_rle_snippet_sync.py` で `cpptemplate`・`paizatemplate` 両方との一致と展開後の動作を検査します。空入力では空の結果を返します。

両テンプレートの `is_prime`・素因数分解・約数列挙・ふるい・除算補助は `library/math/trial_division.hpp` と `integer_division.hpp` に対応し、`python3 tools/check_math_helpers_snippet_sync.py` で両テンプレートとの一致と動作を検査します。`python3 tools/test_template_is_prime.py` でも素数判定の境界を検査します。正の入力の前提、負の除数の修正と無効な除算は各説明を参照してください。

`pairhash` は snippet 内で保守し、`python3 tools/test_pair_hash_snippet.py` で変数名 placeholder を含む全体の展開を検査します。比較元候補と来歴未確認の状況は第三者通知を参照してください。

`to_digit` は snippet 内で保守し、`python3 tools/test_to_digit_snippet.py` で 64 ビット境界を検査します。C++20 が必要です。

`@mos` は 1-indexed 閉区間を入力する操作記入用の例であり、`library/misc/mo.hpp` の独立 API とは別に管理します。`python3 tools/test_mos_snippet.py` は例に区間和の操作を埋めて入力変換・端点移動・ゼロ件を検査します。独立 API は `verify/local/mo.test.cpp` で左右の操作順を検証します。

`@meguru` は predicate を埋める C++20 の二分探索例として管理し、`python3 tools/test_meguru_snippet.py` で端点の差によるオーバーフロー回避を検査します。順列・組合せ・格子方向の例と合わせて [`snippet-examples.md`](snippet-examples.md) に依存と前提を記載します。

`scan` / `in` と両テンプレートの `out_c` / `out_c_pairs` は snippet 内で保守し、`python3 tools/test_template_io_snippet.py` で入れ子入力・失敗時の挙動と出力を検査します。マクロは `python3 tools/test_template_macros.py`、テンプレート全体は ACL の include path を設定した `python3 tools/test_template_full_expansion.py` で展開・実行を検査します。依存と前提は [`template-usage.md`](template-usage.md) を参照してください。

`chmax` / `chmin` と PBDS の単純な alias `ordered_set_less` は snippet 内で保守し、`python3 tools/test_misc_snippets.py` で展開後の動作を検査します。後者は GNU libstdc++ PBDS が必要です。

`next_combination` は両テンプレート内で保守し、`python3 tools/test_next_combination_snippet.py` で定義の一致と、重複・境界を含む全列挙を検査します。旧実装と独立に生成された現実装の来歴は第三者通知を参照してください。

両テンプレートの `_pow` と `mnht` は snippet 内で保守し、`python3 tools/check_math_helpers_snippet_sync.py` で展開後の動作を検査します。累乗は非負指数、距離は中間計算が型に収まることが前提です。

`@segtree_normal`、`@segtree_lazy`、`@segtree_dynamic` は設定例として別管理し、`python3 tools/test_segtree_examples.py` で実際の展開・コンパイルと操作を検査します。使い方と既知の変更は [`segtree-snippets.md`](segtree-snippets.md) に記載します。

## Extracting a snippet

未分離エントリの分類と調査項目は [`snippet-extraction-inventory.md`](snippet-extraction-inventory.md) を参照してください。

1. snippet 本文と隠れた依存（型 alias、macro、using、他 snippet）を読む。
2. 既存 API と意味を保って `library/<category>/name.hpp` へ抽出する。
3. standalone include に必要な標準ヘッダーと名前修飾だけを補う。
4. sibling Markdown に境界、前提、複雑度、既知の legacy 制約を書く。
5. boundary test と決定的 randomized test を追加する。
6. 抽出中に見つけた bug は、互換抽出と同じ変更で黙って直さない。再現例を残し、別変更として扱う。

## Verification

`STANDALONE` は exit code 0 を成功とする local test に使います。`LOCALCASE` は入出力 fixture が本質的な場合、`PROBLEM` は Library Checker、AOJ、yukicoder に実際に適合する問題がある場合だけ使います。Markdown は `documentation_of` でヘッダーに関連付けます。

## 利用者の導線

部品の種類にかかわらず、利用者が「何をしたいか」から [`library-guide.md`](library-guide.md) と生成サイトを通って候補を見つけ、類似部品との選び分け、最小例、前提、所有権、制約、検証結果に到達できるよう維持します。追加・拡張で用途や選定条件が変わったときは、部品文書だけでなく一覧と必要なサイト入口を更新します。

## Provenance and licensing

新しいコードや第三者由来の実装を追加する前に、source URL、確認した版、適用ライセンス、ローカルでの変更内容を記録します。適用範囲は [`../LICENSE-SCOPE.md`](../LICENSE-SCOPE.md)、第三者通知は [`../THIRD_PARTY_NOTICES.md`](../THIRD_PARTY_NOTICES.md)、競技提出時の実務案内は [`license-and-contest-use.md`](license-and-contest-use.md) を参照してください。

`cpp.json` は provenance が混在しています。保守対象の `@interval_set` と `@RBST` は、上流 CC0 部分と rrrriki の Apache-2.0 対象の追加・変更を区別して表示します。`@EulerT` はユーザー申告の来歴（@rrrrikiOW が GitHub Copilot の補助を使って作成）を保持し、具体的な第三者ソースが示されていないことと、Copilot 利用だけでは権利関係の全面保証にならないことを明記します。それ以外の未確認エントリは一括 Apache-2.0 の対象外であり、出典表示がないことをオリジナルまたは自由利用可能である証拠として扱いません。生成文書を配布する場合は、生成に取り込まれるテンプレートの通知を含め、root のライセンス・scope・NOTICE・第三者通知を成果物へ残します。

## Release and publication

ローカルの整理、commit、push、PR、Release、GitHub Pages、repository 設定変更は別の権限です。依頼された範囲を越えて公開しません。Release を作る場合は、対象 commit、tag、添付 digest、公開範囲を事前に確認します。

スニペットの参照URL、検証する commit、Actions の成功条件、Release と添付ファイルの対応は [`release-process.md`](release-process.md) に従います。
