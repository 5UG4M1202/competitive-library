---
title: IntervalSet
documentation_of: ./interval_set.hpp
---

# IntervalSet

値付き半開区間 `[l, r)` を、重複なしで保持する区間 map です。同じ値を持つ隣接区間は更新時に結合されます。`cpp.json` の `@interval_set` を、標準ヘッダーと `std::` 修飾を補って抽出したものです。API と legacy の挙動は変更していません。

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
| `contained_intervals_in(L, R)` | 完全に含まれる保存区間数。ただし下記の legacy 制約あり | `O(log n + k)` |

`n` は保存区間数、`k` は操作で走査・分割・削除する区間数です。

## Callback 付き更新

`update` と `erase` の callback 付き overload は、内部表現から区間を除くたびに `del(L, R, old_value)`、加えるたびに `add(L, R, new_value)` を呼びます。callback は互いに逆操作となるよう設計してください。途中状態も通知されるため、論理的な最終差分だけが通知されるわけではありません。

## 既知の legacy 制約

抽出と修正を混ぜないため、次の点は今回は直していません。

1. 負の端点では、更新用の `Node(l, T{}, value)` sentinel が同じ左端の既存区間を飛び越える場合があります。最小例は `update(-10, -5, 1); update(-10, -7, 2);` です。期待表現は `[-10,-7):2`, `[-7,-5):1` ですが、実際には空区間 `[-10,-10):1` も残ります。このため負端点を契約外としています。
2. `get_mex(p)` は `p` を含む 1 区間の右端までしか進みません。`[0,1):1`, `[1,2):2` に対する数学的な mex は `2` ですが、実際の `get_mex(0)` は被覆済みの `1` を返します。単一値の被覆集合なら隣接区間が結合されるため、その範囲だけを検証済みです。
3. `contained_intervals_in(L, R)` は `lower_bound(Node(L, max, ...))` から走査するため、左端がちょうど `L` の区間を数えません。`[2,3)`, `[3,6)`, `[6,8)` を保存した時、`contained_intervals_in(2,8)` の数学的な結果は `3`、実際の結果は `2` です。この API は intended correctness 未検証・known bug とします。

値に依存しない一般の mex や正しい包含区間数が必要なら、別変更として仕様と failing regression を有効化してから修正してください。

## 検証

- `verify/local/interval_set_boundary.test.cpp`: 分割・結合・削除・callback・コンストラクタ。
- `verify/local/interval_set_randomized.test.cpp`: 非負の有限領域で単純配列との決定的ランダム比較。

boundary test には上記 3 件の「現在挙動」の characterization も含みます。数学的な期待値を要求する assertion は今回は意図的に有効化していません。緑の結果は、既知の bug が修正済みであることを意味しません。

適切に IntervalSet 自体を検証する Library Checker / AOJ 問題はまだ選定していません。無関係な問題を `PROBLEM` に割り当てる代わりに、現時点では `STANDALONE` テストを正とします。

由来: `cpp.json` の `@interval_set`。元コメントに記録されていた参考提出は <https://atcoder.jp/contests/abc435/submissions/71576816> です。
