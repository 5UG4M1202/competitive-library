競技プログラミング用 C++ ヘッダーと、その仕様・検証結果をまとめます。実装説明は各ヘッダーの sibling Markdown にある `documentation_of` から生成されます。

各ライブラリのコード欄では `default` が元のヘッダー、`bundled` が自作ヘッダーの依存を展開した表示です。選択中のコードを `Copy` でコピーできます。外部ライブラリの `#include` は残ります。ライブラリ本体に含まれない利用例の部品は別途必要な場合があるため、各ページの説明も確認してください。テストページのコードは提出用の解答ではありません。

複数ヘッダーを組み合わせる利用例には、必要な自作コードと `main()` をまとめた「提出用の利用例」も用意しています。例の出力と必要な外部ライブラリを確認し、`main()` を問題に合わせて変更してください。

## 収録内容

- [Monoid と ACL セグメント木]({{ '/library/data_structure/monoid_segtree.hpp.html' | relative_url }}): 点更新・区間集約、区間加算・代入・一次変換。どの操作を使うか、初期値と数値範囲に注意
- [動的遅延セグメント木]({{ '/library/data_structure/dynamic_lazy_segtree.hpp.html' | relative_url }}): 広い整数座標での区間更新・集約。初期集約、メモリ所有権、未確認の来歴に注意
- [PrimeUtil]({{ '/library/math/prime_util.hpp.html' | relative_url }}): 上限以下の多数の値の素数判定・素因数分解・約数列挙
- [試し割りと素数表]({{ '/library/math/trial_division.hpp.html' | relative_url }}): 単発の素因数分解・約数列挙とふるい
- [整数除算の丸め]({{ '/library/math/integer_division.hpp.html' | relative_url }}): 正負の除数で切り上げ・切り下げ
- [座標圧縮]({{ '/library/data_structure/coord_compressor.hpp.html' | relative_url }}): 複数配列の値の圧縮・復元。再構築時は以前の対応を置換
- [二次元累積和]({{ '/library/data_structure/cumulative_sum_2d.hpp.html' | relative_url }}): 固定格子の矩形和。座標順序と build のタイミングに注意
- [双対セグメント木]({{ '/library/data_structure/dual_segtree.hpp.html' | relative_url }}): 区間更新と一点取得。作用の合成順序に注意
- [ダブリング]({{ '/library/graph/doubling.hpp.html' | relative_url }}): 次の頂点が一意の遷移を繰り返す。番兵と最大歩数に注意
- [グリッド探索]({{ '/library/graph/grid.hpp.html' | relative_url }}): 壁を避けた上下左右の距離・到達判定
- [二部グラフ判定]({{ '/library/graph/bipartite_graph.hpp.html' | relative_url }}): ACL の dsu による判定・彩色
- [グラフの基本型]({{ '/library/graph/graph.hpp.html' | relative_url }}): 有向・無向の辺を持つ隣接リスト
- [ダイクストラ法]({{ '/library/graph/dijkstra.hpp.html' | relative_url }}): 非負重みの単一始点最短距離
- [K 本の単純最短路]({{ '/library/graph/k_shortest_path.hpp.html' | relative_url }}): 有向グラフで短い経路を複数取得
- [クラスカル法]({{ '/library/graph/kruskal.hpp.html' | relative_url }}): 最小・最大全域森の重み
- [行列の90度回転]({{ '/library/misc/rotate_matrix.hpp.html' | relative_url }}): 長方形の行列・文字グリッドを時計回りに回転
- [Mo のアルゴリズム]({{ '/library/misc/mo.hpp.html' | relative_url }}): オフライン区間クエリを端点移動で処理
- [編集距離]({{ '/library/string/levenstein_distance.hpp.html' | relative_url }}): バイト単位の挿入・削除・置換の最小回数
- [ランレングス圧縮]({{ '/library/string/run_length_encoding.hpp.html' | relative_url }}): 連続する値を個数にまとめる
- [ローリングハッシュ]({{ '/library/string/rolling_hash.hpp.html' | relative_url }}): 部分文字列の比較・結合・共通接頭辞
- [UnionFindWithPotential]({{ '/library/data_structure/union_find_with_potential.hpp.html' | relative_url }}): 頂点間の重みの差を保持し、矛盾する制約を検出
- [IntervalSet]({{ '/library/data_structure/interval_set.hpp.html' | relative_url }}): 値付き半開区間 `[l, r)` の上書き・分割・結合。負の端点と異なる値の隣接区間にも対応
- [LazyReversibleRBST]({{ '/library/data_structure/lazy_reversible_rbst.hpp.html' | relative_url }}): 動的列の挿入・削除・区間集約・作用・反転
- [EulerTour]({{ '/library/graph/euler_tour.hpp.html' | relative_url }}): 根付き木の LCA、辺・頂点のパス和、部分木和、重み更新
- [DistinctKSegments]({{ '/library/data_structure/distinct_k_segments.hpp.html' | relative_url }}): 連続区間分割の異なる値の個数の最大化
- [NQueen]({{ '/library/data_structure/nqueen.hpp.html' | relative_url }}): クイーン配置と探索
- [mydate]({{ '/library/misc/mydate.hpp.html' | relative_url }}): 年 0 と負の年を含む先発グレゴリオ暦の日付演算

## 検証方針

- 各実装について、境界テストと seed 固定のランダム比較を使います。IntervalSet は負の端点や異なる値の隣接区間も検証します。
- 現在は `STANDALONE` テストのみです。各汎用 API 全体に自然に適合する judge-backed problem は未選定です。
- EulerTour の部分木一括加算と根の点更新は正しさ回帰および素朴比較で検証します。緑の検証結果や `LIBRARY_ALL_AC` のアイコンは、記載された範囲のテストが通ったことだけを示し、未検証入力まで保証するものではありません。

人向けのリポジトリ構成、ローカル実行、AI向け保守規則はリポジトリの `README.md` と `AGENTS.md` を参照してください。

## ライセンス

- [Apache License 2.0 本文]({{ '/LICENSE' | relative_url }})
- [適用範囲]({{ '/LICENSE-SCOPE.html' | relative_url }})
- [NOTICE]({{ '/NOTICE' | relative_url }})
- [第三者通知と competitive-verifier の MIT License 本文]({{ '/THIRD_PARTY_NOTICES.html' | relative_url }})

provenance が混在する `cpp.json` 全体は一括 Apache-2.0 の対象外です。IntervalSet と LazyReversibleRBST の上流 CC0 部分は CC0 のままです。EulerTour はユーザー申告の Copilot 補助による作成来歴を記録し、外部の AtCoder Library は CC0 のままです。Apache-2.0 は rrrriki の著作物性のある追加・改変にだけ適用されます。
