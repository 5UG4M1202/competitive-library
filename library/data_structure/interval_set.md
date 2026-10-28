---
title: IntervalSet
documentation_of: ./interval_set.hpp
---

# IntervalSet

値付き半開区間 `[l, r)` を、重複なしで保持する区間 map です。同じ値を持つ隣接区間は更新時に結合されます。元は `cpp.json` の `@interval_set` から抽出し、標準ヘッダーと `std::` 修飾を補ったものです。現在はこのヘッダーを保守対象の正本とし、対応 snippet はテストと文書を含む変更後に同期します。公開 API は維持し、抽出後に確認された bug は回帰テストとともに個別に修正します。

## 契約

- `T` は順序、差、加算、`std::numeric_limits<T>::max()` を利用できる整数型を想定します。
- 現行の legacy 実装は更新検索に `T{}`、すなわち整数型では `0` を一時的な右端 sentinel として使います。安全な利用範囲は `0 <= l <= r` です。負の端点はサポート対象外です。
- `r` は含みません。空区間 `[x, x)` の更新・削除は何もしません。
- `r - l` と被覆長の総和が `T` で表現できる必要があります。オーバーフローは検出しません。
- `identity` は未被覆点に対する `get_val` の返り値です。区間値として同じ値を保存することもできるため、存在判定には `covered` または `get` を使います。
- `insert(l, r)` が保存する値は `VAL()` です。コンストラクタに渡した `identity` とは限りません。
- `covered(l, r)` は範囲全体が「1 個の保存区間」に含まれるかを判定します。異なる値の隣接区間をまたぐ範囲には `false` を返します。

## 主な操作

| 操作 | 意味 | 計算量 |
| --- | --- | --- |
| `update(l, r, value)` | `[l, r)` を上書き | `O(log n + k)` |
| `erase(l, r)` | `[l, r)` の被覆を削除 | `O(log n + k)` |
| `get(p)`, `get_val(p)`, `covered(p)` | 点を含む区間・値・存在 | `O(log n)` |
| `lower_bound(p)` | `p` を含む区間、なければ右側の最初の区間 | `O(log n)` |
| `covered_all()` | 全区間の被覆長 | `O(1)` |
| `covered_length_in(L, R)` | `[L, R)` 内の被覆長 | `O(log n + k)` |
| `contained_intervals_in(L, R)` | `[L, R)` に完全に含まれる保存区間数 | `O(log n + k)` |

`n` は保存区間数、`k` は操作で走査・分割・削除する区間数です。

## Callback 付き更新

`update` と `erase` の callback 付き overload は、内部表現から区間を除くたびに `del(L, R, old_value)`、加えるたびに `add(L, R, new_value)` を呼びます。callback は互いに逆操作となるよう設計してください。途中状態も通知されるため、論理的な最終差分だけが通知されるわけではありません。

## 既知の legacy 制約

次の点は引き続き legacy 制約として残しています。

1. 負の端点では、更新用の `Node(l, T{}, value)` sentinel が同じ左端の既存区間を飛び越える場合があります。最小例は `update(-10, -5, 1); update(-10, -7, 2);` です。期待表現は `[-10,-7):2`, `[-7,-5):1` ですが、実際には空区間 `[-10,-10):1` も残ります。このため負端点を契約外としています。
2. `get_mex(p)` は `p` を含む 1 区間の右端までしか進みません。`[0,1):1`, `[1,2):2` に対する数学的な mex は `2` ですが、実際の `get_mex(0)` は被覆済みの `1` を返します。単一値の被覆集合なら隣接区間が結合されるため、その範囲だけを検証済みです。
値に依存しない一般の mex が必要なら、別変更として仕様、計算量、failing regression を合意してから修正してください。

## 検証

- `verify/local/interval_set_boundary.test.cpp`: 分割・結合・削除・callback・コンストラクタ。
- `verify/local/interval_set_randomized.test.cpp`: 非負の有限領域で単純配列との決定的ランダム比較。

boundary test には上記 2 件の「現在挙動」の characterization を残し、`contained_intervals_in` には左端一致を含む intended-correctness regression を追加しています。randomized test は保存区間の境界を列挙し、単純な包含数オラクルと比較します。緑の結果は、残る 2 件の legacy 制約が修正済みであることを意味しません。

適切に IntervalSet 自体を検証する Library Checker / AOJ 問題はまだ選定していません。無関係な問題を `PROBLEM` に割り当てる代わりに、現時点では `STANDALONE` テストを正とします。

## 由来とライセンス

`cpp.json` の `@interval_set` から抽出しました。元コメントに記録されていた参考提出は <https://atcoder.jp/contests/abc435/submissions/71576816> です。

IntervalSet の中核は [drken1215/algorithm の `intervals_set.cpp`](https://github.com/drken1215/algorithm/blob/79b5a6a826690c4d8e0bd64e812e85b5f425ea6b/DataStructureBasic/intervals_set.cpp) を参考・改変した可能性が高く、上流は CC0 1.0 Universal です。`Node`、vector コンストラクタ、検索、mex、callback 付き update/erase、`emplace_hint` を使う分割・結合順序に特徴的な共通点があります。リンク先は今回比較した版であり、実際に参照した正確な過去 revision は確定していません。

ローカルでの主な追加・変更は、standalone include、`std::` 修飾、const overload、`total_cover`、`covered_all`、`covered_length_in`、`contained_intervals_in`、文書、検証テストです。元の CC0 部分は CC0 のまま利用でき、Apache-2.0 は rrrriki の著作物性のある追加・改変だけに適用されます。詳細は [`../../THIRD_PARTY_NOTICES.md`](../../THIRD_PARTY_NOTICES.md) と [`../../LICENSE-SCOPE.md`](../../LICENSE-SCOPE.md) を参照してください。
