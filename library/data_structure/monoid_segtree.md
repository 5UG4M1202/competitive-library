---
title: Monoid と ACL セグメント木
documentation_of: ./monoid_segtree.hpp
---

# Monoid と ACL セグメント木

値の結合と区間操作の定義を型にまとめ、ACL の通常・遅延セグメント木に渡します。ACL のヘッダーが必要です。本体は ACL の `atcoder::segtree` / `atcoder::lazy_segtree` であり、添字・計算量・`max_right` 等の仕様も ACL に従います。ここで使う範囲は半開区間 `[l, r)` です。

## 操作から選ぶ

| やりたいこと | 型 |
| --- | --- |
| 点変更と区間和・最小・最大 | `cp::Segtree<cp::RangeAggregate<T, K>>` |
| 区間加算と区間和・最小・最大 | `cp::LazySegtree<cp::RangeAdd<T, K>>` |
| 区間代入と区間和・最小・最大 | `cp::LazySegtree<cp::RangeAssign<T, K>>` |
| 区間一次変換 `x → a*x+b` と区間和 | `cp::LazySegtree<cp::RangeAffineSum<T>>` |

`K` は `cp::AggregateKind::sum`、`minimum`、`maximum` です。区間代入の作用は `std::optional<T>` で表し、`std::nullopt` が何もしない作用です。入力値と衝突する特殊な整数を使いません。一次変換 `{1, b}` は加算、`{0, b}` は代入、`{a, 0}` は乗算を表します。

## 最小例：区間加算と区間和

```cpp
#include "library/data_structure/monoid_segtree.hpp"

using P = cp::RangeAdd<long long, cp::AggregateKind::sum>;
std::vector<long long> a{1, 2, 3};
cp::LazySegtree<P> seg(cp::range_values<P>(a));
seg.apply(0, 2, 5);               // {6, 7, 3}
auto sum = seg.prod(0, 3).value;  // 16
seg.set(1, P::leaf(4));          // {6, 4, 3}
```

`value_type` は `{value, length}` です。`P::leaf(x)` は長さ1の要素、`P::id()` は長さ0の空区間で、`prod(l,l)` は後者です。`cp::LazySegtree<P> seg(n)` は ACL の仕様により各要素を `P::id()` で初期化するため、**長さ1の値0が n 個ある配列にはなりません**。通常は `range_values<P>(vector<T>(n, 0))` などで構築してください。最小・最大の空区間値は `numeric_limits<T>::max()` / `lowest()` です。空かどうかは `length` で判定できます。

## 一次変換と合成順序

```cpp
using P = cp::RangeAffineSum<long long>;
cp::LazySegtree<P> seg(cp::range_values<P>(std::vector<long long>{1, 2}));
seg.apply(0, 2, {2, 1}); // x -> 2*x+1
seg.apply(0, 2, {3, 4}); // x -> 3*x+4 （後に作用）
// 結果は {13, 19}
```

`composition(f, g)` は `f(g(x))`、つまり `g` の後に `f` を適用します。逆にすると代入・アフィン変換等で間違います。`RangeAssign` では後からの代入が優先されます。

## 新しい定義を作る

通常木用の型 `M` は `value_type`、`static value_type op(value_type, value_type)`、`static value_type id()` を用意します。遅延木用の型 `A` はさらに `action_type`、`mapping(action_type, value_type)`、`composition(action_type, action_type)`、`action_id()` を同じ値渡しの形で用意します。`leaf` と `range_values` はこのヘッダーの便宜機能で、アダプターそのものの必須要件ではありません。

例えば、点変更・区間 XOR 用の定義は次のように追加できます。

```cpp
struct Xor {
  using value_type = unsigned;
  static unsigned op(unsigned a, unsigned b) { return a ^ b; }
  static unsigned id() { return 0; }
};
cp::Segtree<Xor> seg(std::vector<unsigned>{1, 2, 3});
// seg.prod(0, 3) == (1 ^ 2 ^ 3)
```

このような独自型は組み込みの符号付き整数ポリシーとは独立しているため、ACL の要件を満たせば `unsigned` や modint なども利用できます。

`op` の結合律と単位元、作用の恒等性・合成順・結合との両立 `mapping(f, op(x,y)) = op(mapping(f,x), mapping(f,y))` を確認します。たとえば和に加算を適用する場合、区間和だけでは要素数が分からず、長さを持たせる必要があります。使用する状態と作用の集合は、これらの法則の下で閉じている必要があります。詳細は [ACL の要件](https://atcoder.github.io/ac-library/production/document_ja/lazysegtree.html)を参照してください。

追加候補には、gcd、bitwise 演算、文字列連結、最小値とその出現回数、最大部分和、区間代入と文字数カウント、modint による一次変換があります。どの作用が使えるかは保持する情報次第です。例えば単なる区間和だけから一括 `chmin` 後の和を求めることはできず、別のデータ構造や追加情報が必要です。非可換な演算では `op(left, right)` の順序を守り、逆順でも検証してください。

各組み込みポリシーは符号付き整数型を想定し、長さと、結合・適用・作用の合成のすべての中間結果が型の範囲に収まることが前提です。オーバーフローは検出しません。`length` は `long long`、和の場合は `T` に変換されるので長さも `T` で表現可能にしてください。`int` の端点とメモリの制約は ACL に従います。一次変換には modular arithmetic が必要な場合が多く、標準整数の代わりに利用する型に応じた独自ポリシーを定義してください。

構築は `O(n)`、点操作・区間操作は `O(log n)`、メモリは `O(n)`（いずれも定義した演算が定数時間の場合）です。[ローカル検証](../../verify/local/monoid_segtree.test.cpp)は6種類と一次変換の合成を確認します。
