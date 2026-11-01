# Local tests

## EulerTour

- `euler_tour_boundary.test.cpp`: LCA、深さ、辺・頂点のパス和、部分木和、点更新、両表現への部分木加算、根の点更新、単頂点木を intended-correctness として確認します。
- `euler_tour_randomized.test.cpp`: seed 固定でランダム木を作り、LCA、パス和、部分木和、根を含む頂点点更新、辺点更新、両表現への部分木加算を素朴実装と比較します。
- AtCoder Library の `segtree` / `lazysegtree` が必要です。`Graph` はテスト内の最小互換型を使います。
- `0-1`, `1-2`, `1-3` の重み `0` の木で部分木 `1` へ `10` を両表現に加算し、`query_vert(2,3)` と `query_edge(2,3)` がともに `20` になる回帰を含みます。以前の `30` を受け入れる characterization は削除しました。

## LazyReversibleRBST

- `lazy_reversible_rbst_boundary.test.cpp`: 空木、非空 build、split/merge、fold、apply、reverse、insert、erase の intended-correctness。
- `lazy_reversible_rbst_randomized.test.cpp`: seed 固定 5,000 operation で `std::vector<long long>` と動的列操作・区間和を比較する intended-correctness。
- legacy 前提である非空 `build`、非空区間への `apply`、有効添字だけを検証します。範囲外入力を安全化したと解釈しません。

## IntervalSet

## Intended-correctness coverage

- 負の端点を含む上書き、分割、同値区間の結合、削除。
- 点・1 区間被覆、値取得、被覆長、callback 差分。
- 左端・右端が保存区間境界と一致する範囲を含む、完全包含区間数。
- 値の異なる隣接区間を含む被覆集合の `get_mex`。
- 単純配列モデルとの seed 固定 5,000 operation 比較。
- `interval_set_signed.test.cpp` では符号付き座標での更新・削除・照会と callback を配列モデルと比較します。
