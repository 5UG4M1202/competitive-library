---
title: LazyReversibleRBST
documentation_of: ./lazy_reversible_rbst.hpp
---

# LazyReversibleRBST

列を暗黙的な添字で保持し、挿入・削除・区間集約・区間作用・区間反転を扱う乱択平衡二分木です。`cpp.json` の `@RBST` を、標準ヘッダーと `std::` 修飾を補って分離しました。公開型名と操作は維持しています。

## 使う場面

セグメント木の区間集約・遅延作用に加えて、列の途中への挿入・削除や区間反転も必要なときに使います。列長が固定で挿入・削除・反転を行わない場合は、計算量が決定的で定数倍も小さい通常のセグメント木を優先できます。

## 最小利用例

区間和・区間加算・区間反転を組み合わせる例です。リポジトリルートを include path に含めて C++20 でコンパイルしてください。`g` は1要素と部分木集約の両方へ適用されるため、`Summary` に要素数を保持します。

```cpp
#include <iostream>

#include "library/data_structure/lazy_reversible_rbst.hpp"

struct Summary {
  long long sum = 0;
  int length = 0;
  friend bool operator==(const Summary&, const Summary&) = default;
};

struct Add {
  long long value = 0;
  friend bool operator==(const Add&, const Add&) = default;
};

Summary combine(Summary left, Summary right) {
  return {left.sum + right.sum, left.length + right.length};
}

Summary map_add(Summary summary, Add action) {
  summary.sum += action.value * summary.length;
  return summary;
}

Add compose_add(Add old_action, Add new_action) {
  return {old_action.value + new_action.value};
}

Summary reverse_summary(Summary summary) { return summary; }

using Tree = LazyReversibleRBST<Summary, Add, combine, map_add, compose_add,
                                reverse_summary>;

Summary value(long long x) { return {x, 1}; }

int main() {
  Tree tree;
  Tree::Ptr root = tree.make_tree();

  tree.insert(root, 0, value(1));
  tree.insert(root, 1, value(2));
  tree.insert(root, 2, value(3));

  tree.apply(root, 0, 2, Add{10});  // [11, 12, 3]
  tree.reverse(root, 0, 3);         // [3, 12, 11]

  std::cout << tree.fold(root, 0, 3).sum << '\n';
  for (int i = 0; i < tree.size(root); ++i) {
    if (i != 0) std::cout << ' ';
    std::cout << tree.fold(root, i, i + 1).sum;
  }
  std::cout << '\n';

  // 全ノードの所有権は呼び出し側にあるため、使用後に解放する。
  while (root != nullptr) tree.erase(root, 0);
}
```

出力:

```text
26
3 12 11
```

この例と同じ種類の区間加算・区間和・反転・解放は `verify/local/lazy_reversible_rbst_boundary.test.cpp` でも assertion 付きで検証しています。verifier のページでは「Verified with」からテストを開けます。

## テンプレート引数

- `T`: 1要素と部分木集約の型。`T()` が集約の単位元である必要があります。
- `E`: 遅延作用の型。`E()` が作用の単位元で、`operator==` / `operator!=` で単位元判定できる必要があります。
- `f(left, right)`: 結合順を保つ結合則的な集約。
- `g(value_or_summary, action)`: 1要素または部分木集約へ作用を適用する関数。部分木長が必要なら `T` 自身に長さを含めます。
- `h(old_action, new_action)`: 既存作用の後に新しい作用を合成します。
- `ts(summary)`: 区間を反転したときの集約を返します。可換な集約なら恒等関数で構いません。

これらの演算は、`f` の結合則・`T()` の単位元、`h` の合成と `E()` の単位元、`g` の集約への分配と合成との整合性を満たす必要があります。`ts` は反転の順序を表し、`ts(f(a, b)) = f(ts(b), ts(a))`、二度の反転で元に戻ること、および遅延作用との整合性も必要です。任意の関数を渡せば正しく動くわけではありません。

## 操作と計算量

| 操作 | 意味 | 期待計算量 |
| --- | --- | --- |
| `make_tree()` | 空木を作る | `O(1)` |
| `build(values)` | 非空列から木を構築する | `O(n)` |
| `size(root)` | 要素数 | `O(1)` |
| `split(root, k)` | `[0,k)` と `[k,n)` に分ける | `O(log n)` |
| `merge(left, right)` | 左列と右列を連結する | `O(log n)` |
| `insert(root, k, value)` | 添字 `k` の直前へ挿入する | `O(log n)` |
| `erase(root, k)` | 添字 `k` を削除する | `O(log n)` |
| `fold(root, l, r)` | 半開区間 `[l,r)` を集約する | `O(log n)` |
| `apply(root, l, r, action)` | `[l,r)` に作用を適用する | `O(log n)` |
| `reverse(root, l, r)` | `[l,r)` を反転する | `O(log n)` |

乱択木なので計算量は期待値です。決定的な疑似乱数列を使い、最悪時には高さと各操作が `O(n)` になり得ます。メモリは `O(n)`、再帰深さは期待 `O(log n)` です。

## 前提と legacy 制約

- 添字は常に現在の列長に対して有効でなければなりません。`split` / `insert` は `0 <= k <= n`、`erase` は `0 <= k < n`、区間操作は `0 <= l <= r <= n` が前提です。
- `build(values)` は空でない `values` が前提です。空木には `make_tree()` を使います。
- legacy 実装の `apply` は空区間で null node を `propagate` するため、`l < r` が必要です。`fold` と `reverse` は空区間を扱えます。
- raw pointer を公開する既存 API を維持しています。`erase` した node は解放されますが、木全体を自動解放する destructor はありません。長寿命プロセスでは利用者側で全 node を解放してください。
- `insert`、`erase`、`fold`、`apply`、`reverse` は root を参照渡しで更新します。`split(root, k)` は元の木を破壊的に分割するので、元の `root` を使い続けず、戻り値の2つを所有してください。`merge(left, right)` も入力の木を組み替えます。返された root を使い、古い root を別の木として再利用・解放しないでください。
- `g` は要素と集約の両方に同じ形で適用されます。区間加算・区間和のように長さが必要な作用では、検証コードのように `T` に要素数を保持します。
- 範囲外添字、空の `build`、空区間への `apply` は検証対象外で、入力検査も行いません。

## 検証

- `verify/local/lazy_reversible_rbst_boundary.test.cpp`: build、split/merge、fold、apply、reverse、insert、erase と空木。
- `verify/local/lazy_reversible_rbst_randomized.test.cpp`: seed 固定で `std::vector<long long>` と挿入・削除・加算・反転・区間和を比較。

この汎用 API 全体を自然に要求する judge-backed problem はまだ選定していないため、現時点では `STANDALONE` 検証を正とします。

## 出典とライセンス

中核は NyaanNyaan/library の [`rbst-base.hpp`](https://github.com/NyaanNyaan/library/blob/b3981adc80a800b2584980b01821324ea6c77183/rbst/rbst-base.hpp) と [`lazy-reversible-rbst.hpp`](https://github.com/NyaanNyaan/library/blob/b3981adc80a800b2584980b01821324ea6c77183/rbst/lazy-reversible-rbst.hpp) に由来します。確認版 `b3981adc80a800b2584980b01821324ea6c77183` のリポジトリライセンスは CC0 1.0 Universal です。

上流部分は CC0 のまま利用できます。standalone include、`std::` 修飾、文書、検証、同期検査など rrrriki の著作物性のある追加・変更だけが Apache-2.0 の対象です。詳細は [`../../THIRD_PARTY_NOTICES.md`](../../THIRD_PARTY_NOTICES.md) と [`../../LICENSE-SCOPE.md`](../../LICENSE-SCOPE.md) を参照してください。
