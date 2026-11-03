---
title: 転倒数
documentation_of: ./inversion_count.hpp
---

# 転倒数

列全体の転倒数を、座標圧縮を含めて求めます。`cp::inversion_count(values)` は `i < j` かつ `values[i] > values[j]` の組の数です。等しい値同士は数えず、入力は変更しません。空列の答えは 0 です。

```cpp
#include "library/data_structure/inversion_count.hpp"
#include <iostream>
#include <vector>
int main() {
  std::vector<long long> a{3, 1, 2, 1};
  std::cout << cp::inversion_count(a) << '\n';  // 4
  auto ending = cp::inversion_count_ending_at(a);
  auto starting = cp::inversion_count_starting_at(a);
  for (std::size_t i = 0; i < a.size(); ++i) std::cout << (i ? " " : "") << ending[i];
  std::cout << '\n';  // 0 1 1 2
  for (std::size_t i = 0; i < a.size(); ++i) std::cout << (i ? " " : "") << starting[i];
  std::cout << '\n';  // 3 0 1 0
}
```

検証ページの「提出用の利用例」は本体と `main()` をまとめてコピーできます。ACL の `#include` は残るので、提出先で ACL が使えることを確認し、入力部分を問題に合わせて変更してください。

比較可能な値の `std::vector<T>` を受け取ります。第 2 引数に比較関数 `comp` を渡すと、`comp(values[j], values[i])` が真の組を数えます。比較は狭義弱順序で、`T` はコピー・並べ替え可能でなければなりません。浮動小数点に NaN が混じるとこの前提を満たしません。

要素ごとの寄与も、元の添字順の `std::vector<long long>` で取得できます。

- `inversion_count_ending_at(a)[j]` は `i < j` かつ `a[i] > a[j]` となる左側の要素の数です。
- `inversion_count_starting_at(a)[i]` は `i < j` かつ `a[i] > a[j]` となる右側の要素の数です。

それぞれを全添字で合計すると、どちらも `inversion_count(a)` に一致します。ある要素が転倒の一方の端として関わる組の総数は、その添字の両者の和です。比較関数を指定した場合も上記の大小をその比較順に従って判断します。空列ではいずれも空の配列を返します。

いずれの関数も計算量はそれぞれ `O(n log n)`、追加メモリは `O(n)` です。両方の寄与を取得する場合はそれぞれ独立に座標圧縮・計算します。内部で [ACL Fenwick Tree](https://atcoder.github.io/ac-library/production/document_ja/fenwicktree.html) を使うため ACL が必要で、ACL の制約に合わせて `n <= 100000000` とします。超過時は `std::length_error` を送出します。この範囲では最大 `n(n-1)/2` は戻り値 `long long` に収まります。メモリ制限にも注意してください。

列全体を一度だけ求める用途の関数です。部分区間ごとに独立して呼ぶとその区間の長さに応じた計算が毎回必要です。大量の静的区間クエリなら、左右別の追加・削除で転倒数を保つ [Mo のアルゴリズム](../misc/mo.md) を検討してください。要素の変更には対応しません。

ローカルテストでは空列、重複、負数、逆順の比較と、二重ループによる固定 seed のランダム比較を行います。[AOJ ALDS1_5_D](https://onlinejudge.u-aizu.ac.jp/problems/ALDS1_5_D) でも列全体の転倒数を検証します。AOJ は値がすべて異なる制約のため、重複はローカルで別途検証します。
