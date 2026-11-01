# Data structures

| 説明 | ヘッダー | 概要 | 検証 |
| --- | --- | --- | --- |
| [双対セグメント木](dual_segtree.md) | [dual_segtree.hpp](dual_segtree.hpp) | 区間更新と一点取得 | [local tests](../../verify/local/dual_segtree.test.cpp) |
| [二次元累積和](cumulative_sum_2d.md) | [cumulative_sum_2d.hpp](cumulative_sum_2d.hpp) | 静的な格子の矩形和 | [local tests](../../verify/local/cumulative_sum_2d.test.cpp) |
| [座標圧縮](coord_compressor.md) | [coord_compressor.hpp](coord_compressor.hpp) | 複数配列の値をまとめて圧縮・復元 | [local tests](../../verify/local/coord_compressor.test.cpp) |
| [Monoid と ACL セグメント木](monoid_segtree.md) | [monoid_segtree.hpp](monoid_segtree.hpp) | 区間和・最小・最大と加算・代入、一次変換 | [local tests](../../verify/local/monoid_segtree.test.cpp) |
| [動的遅延セグメント木](dynamic_lazy_segtree.md) | [dynamic_lazy_segtree.hpp](dynamic_lazy_segtree.hpp) | 広い整数座標で区間更新・集約・境界探索 | [local tests](../../verify/local/dynamic_lazy_segtree.test.cpp) |
| [`LazyReversibleRBST`](lazy_reversible_rbst.md) | [`lazy_reversible_rbst.hpp`](lazy_reversible_rbst.hpp) | 動的列の挿入・削除・区間集約・作用・反転 | [local tests](../../verify/local/) |
| [`IntervalSet`](interval_set.md) | [`interval_set.hpp`](interval_set.hpp) | 値付き半開区間 `[l, r)` の上書き・分割・結合 | [local tests](../../verify/local/) |
| [`DistinctKSegments`](distinct_k_segments.md) | [`distinct_k_segments.hpp`](distinct_k_segments.hpp) | 連続区間分割の異なる値の個数の最大化 | [local tests](../../verify/local/) |
| [`NQueen`](nqueen.md) | [`nqueen.hpp`](nqueen.hpp) | Nクイーンの配置と探索 | [local tests](../../verify/local/) |
| [`UnionFindWithPotential`](union_find_with_potential.md) | [`union_find_with_potential.hpp`](union_find_with_potential.hpp) | 頂点間の重みの差を管理 | [local tests](../../verify/local/) |

Union-Find は AtCoder Library の `atcoder::dsu` を利用し、重複実装をここには追加しません。
