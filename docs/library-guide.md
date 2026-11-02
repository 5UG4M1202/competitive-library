# やりたいことから探す

木の非可換なパス集約・方向別の辺値には [重軽分解](../library/graph/heavy_light_decomposition.md) を利用できます。[設計案](hld-design.md)には背景と未対応事項を記録しています。

| 要件 | 候補 | はじめに確認すること |
| --- | --- | --- |
| 点更新・区間集約 | [Monoid と ACL セグメント木](../library/data_structure/monoid_segtree.md) | `op` の順序、単位元、初期値 |
| 整数・実数の固定 N 次元点 | [N 次元の点](../library/geometry/point.md) | 座標型・中間型とオーバーフロー |
| 整数点の向きや偏角順 | [整数点と偏角](../library/geometry/integer_point.md) | 軸・ゼロ・同方向と中間結果の範囲 |
| 整数点集合の凸包 | [整数座標の凸包](../library/geometry/integer_convex_hull.md) | 重複点、辺上の点を残すか、整数の範囲 |
| 凸包の点の内外・境界 | [整数凸包への点の包含判定](../library/geometry/integer_convex_contains.md) | 反時計回りの厳密な凸包、辺上の点 |
| 整数点の最も離れた2点 | [整数点集合の最遠点対](../library/geometry/integer_farthest_pair.md) | 元の添字、重複点、二乗距離の範囲 |
| 整数点の最も近い2点 | [整数点集合の最近点対](../library/geometry/integer_closest_pair.md) | 元の添字、重複点、二乗距離の範囲 |
| 固定した整数点の矩形集約・点の値更新 | [矩形集約 kd-tree](../library/geometry/integer_kd_tree.md) | 可換 Monoid、半開矩形、最悪線形 |
| 固定した実数座標の矩形集約・点の値更新 | [実数座標の矩形集約 kd-tree](../library/geometry/real_kd_tree.md) | 有限座標、丸め、半開矩形 |
| 実数点の向きや偏角順 | [実数点と偏角](../library/geometry/real_point.md) | EPS、丸め、比較順 |
| 実数点集合の凸包 | [実数座標の凸包](../library/geometry/real_convex_hull.md) | 重複・一直線・EPS |
| 実数凸包の点の包含判定 | [実数凸包への点の包含判定](../library/geometry/real_convex_contains.md) | 厳密凸・反時計回り |
| 実数点集合の最近・最遠点対 | [最近点対](../library/geometry/real_closest_pair.md)・[最遠点対](../library/geometry/real_farthest_pair.md) | EPS、距離の丸め |
| 点更新・区間 gcd または bitwise AND・OR・XOR | [Monoid と ACL セグメント木](../library/data_structure/monoid_segtree.md) | 符号なし整数、演算ごとの空区間の値 |
| 点更新・区間最小値とその出現回数 | [Monoid と ACL セグメント木](../library/data_structure/monoid_segtree.md) | 空区間は `count == 0`、要素は `leaf` で構築 |
| 区間更新と一点取得 | [双対セグメント木](../library/data_structure/dual_segtree.md) | 作用の合成順序、初期値と作用の型 |
| 区間加算・代入と和・最小・最大 | [Monoid と ACL セグメント木](../library/data_structure/monoid_segtree.md) | `[l,r)`、要素数、オーバーフロー |
| 区間一次変換と和 | [Monoid と ACL セグメント木](../library/data_structure/monoid_segtree.md) | 合成順序、数値範囲 |
| 競技用テンプレートでセグメント木を使う | [セグメント木スニペット](segtree-snippets.md) | 定義を置く場所、初期値、用途ごとの違い |
| 広い整数座標上の区間更新・集約 | [動的遅延セグメント木](../library/data_structure/dynamic_lazy_segtree.md) | 初期区間集約、ノード所有権、来歴未確認 |
| 列の挿入・削除・反転と区間操作 | [LazyReversibleRBST](../library/data_structure/lazy_reversible_rbst.md) | 可逆な集約、作用の条件 |
| 値付き区間の分割・上書き | [IntervalSet](../library/data_structure/interval_set.md) | 半開区間、番兵、値の境界 |
| 重みの差を持つ連結成分 | [UnionFindWithPotential](../library/data_structure/union_find_with_potential.md) | `diff(x,y)` の符号、矛盾判定 |
| 通常の連結成分 | [ACL dsu](https://atcoder.github.io/ac-library/production/document_ja/dsu.html) | 頂点の範囲、`merge` の戻り値 |
| 複数配列をまとめて座標圧縮・復元 | [座標圧縮](../library/data_structure/coord_compressor.md) | 値の型の条件、再 build 時の対応の置換 |
| 固定された格子で矩形和を繰り返し取得 | [二次元累積和](../library/data_structure/cumulative_sum_2d.md) | 座標順序、先に一度 build、メモリ |
| 上限以下の多数の値の素数判定・素因数分解・約数列挙 | [PrimeUtil](../library/math/prime_util.md) | 表の上限とメモリ、入力範囲 |
| 単発の素因数分解・約数列挙や素数表 | [試し割りと素数表](../library/math/trial_division.md) | 正の整数、試し割りの計算量 |
| 整数除算の切り上げ・切り下げ | [整数除算の丸め](../library/math/integer_division.md) | 負の除数、0 除算、型の限界 |
| 根付き木の LCA・パス・部分木 | [EulerTour](../library/graph/euler_tour.md) | 木の構築、重みと区間の扱い |
| 非可換なパス集約・方向別の辺の値 | [重軽分解](../library/graph/heavy_light_decomposition.md) | 根、頂点・辺、往復の値、結合順序 |
| 次の頂点が一意の遷移を繰り返す | [ダブリング](../library/graph/doubling.md) | 行き止まりの番兵、最大歩数と構築するビット数 |
| 壁のあるグリッドの上下左右の最短距離・到達判定 | [グリッド探索](../library/graph/grid.md) | 座標の順序、壁、訪問済み状態 |
| 二部グラフの判定と彩色 | [二部グラフ判定](../library/graph/bipartite_graph.md) | 辺の形式、彩色できる条件、ACL 依存 |
| グラフを作り有向・無向の辺を追加 | [グラフの基本型](../library/graph/graph.md) | 頂点番号、辺番号、重み |
| 非負重みの最短距離 | [ダイクストラ法](../library/graph/dijkstra.md) | 負辺不可、距離型と未到達の番兵 |
| 複数の短い単純路 | [K 本の単純最短路](../library/graph/k_shortest_path.md) | 有向辺の番号、非負重み、探索量 |
| 最小・最大全域木の重み | [クラスカル法](../library/graph/kruskal.md) | 非連結なら全域森、ACL 依存 |
| N クイーン探索 | [NQueen](../library/data_structure/nqueen.md) | サイズと探索量 |
| 区間分割と異なる値の数 | [DistinctKSegments](../library/data_structure/distinct_k_segments.md) | 分割数と計算量 |
| 日付の比較と演算 | [mydate](../library/misc/mydate.md) | 日付範囲と暦の前提 |
| 行列・文字グリッドを90度回転 | [行列の90度回転](../library/misc/rotate_matrix.md) | 空行列・不揃いな行、文字列のバイト単位の扱い |
| オフライン区間クエリを端点の追加・削除で処理 | [Mo のアルゴリズム](../library/misc/mo.md) | 半開区間、左右別の操作、回答順 |
| 文字列の挿入・削除・置換の最小回数 | [編集距離](../library/string/levenstein_distance.md) | バイト単位、二次元表のメモリ |
| 連続する同じ値をまとめる | [ランレングス圧縮](../library/string/run_length_encoding.md) | 空入力、文字列のバイト単位 |
| 部分文字列の比較・結合・共通接頭辞 | [ローリングハッシュ](../library/string/rolling_hash.md) | 衝突、同じ base、バイト単位 |

必要な操作が表になければ、各部品の条件と計算量、外部依存、既存 API を調べてから追加します。名前が似ていても点更新と区間更新など対応機能は同じとは限りません。

入力や処理を埋める二分探索・順列・格子方向のスニペットは [操作例スニペット](snippet-examples.md) を参照してください。

`chmax` / `chmin`、GNU PBDS の集合 alias、入出力補助は [テンプレートの使い方](template-usage.md) を参照してください。
