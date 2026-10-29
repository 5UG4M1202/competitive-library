# Data structures

| 説明 | ヘッダー | 概要 | 検証 |
| --- | --- | --- | --- |
| [`LazyReversibleRBST`](lazy_reversible_rbst.md) | [`lazy_reversible_rbst.hpp`](lazy_reversible_rbst.hpp) | 動的列の挿入・削除・区間集約・作用・反転 | [local tests](../../verify/local/) |
| [`IntervalSet`](interval_set.md) | [`interval_set.hpp`](interval_set.hpp) | 値付き半開区間 `[l, r)` の上書き・分割・結合 | [local tests](../../verify/local/) |
| [`DistinctKSegments`](distinct_k_segments.md) | [`distinct_k_segments.hpp`](distinct_k_segments.hpp) | 連続区間分割の異なる値の個数の最大化 | [local tests](../../verify/local/) |
| [`NQueen`](nqueen.md) | [`nqueen.hpp`](nqueen.hpp) | Nクイーンの配置と探索 | [local tests](../../verify/local/) |

Union-Find は AtCoder Library の `atcoder::dsu` を利用し、重複実装をここには追加しません。
