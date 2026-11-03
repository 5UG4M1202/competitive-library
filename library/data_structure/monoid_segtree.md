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
| 点変更と区間 gcd | `cp::Segtree<cp::Gcd<T>>` |
| 点変更と区間 bitwise AND・OR・XOR | `cp::Segtree<cp::Bitwise<T, K>>` |
| 点変更と区間最小値・その出現回数 | `cp::Segtree<cp::MinCount<T>>` |

`K` は `cp::AggregateKind::sum`、`minimum`、`maximum` です。区間代入の作用は `std::optional<T>` で表し、`std::nullopt` が何もしない作用です。入力値と衝突する特殊な整数を使いません。一次変換 `{1, b}` は加算、`{0, b}` は代入、`{a, 0}` は乗算を表します。

gcd と bitwise は `bool` を除く標準の符号なし整数型を使用します。gcd の空区間は `0`（`gcd(0,0)=0`）、bitwise AND はその型の全ビット 1、OR・XOR は `0` です。bitwise の `K` は `cp::BitwiseKind::bit_and`、`bit_or`、`bit_xor` のいずれかです。これらは点更新・区間取得用のモノイドであり、区間更新の作用は定義していません。

`MinCount<T>` は `bool` を除く標準の整数型に対応し、`{value, count}` を返します。空区間は `{numeric_limits<T>::max(), 0}`、要素は `leaf(x)` で作り、空かどうかは `count == 0` で判定します。最大値そのものが入力に含まれていても、回数で空区間と区別できます。回数の合計は `long long` に収まる必要があります。

```cpp
using G = cp::Gcd<unsigned long long>;
cp::Segtree<G> gcd(std::vector<unsigned long long>{12, 18, 30});
gcd.set(1, 24);
// gcd.prod(0, 3) == 6

using Min = cp::MinCount<long long>;
cp::Segtree<Min> minima(cp::range_values<Min>(std::vector<long long>{3, 1, 1}));
auto result = minima.prod(0, 3); // result.value == 1, result.count == 2
```

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

## Monoid 定義ガイド

Monoid は「複数の値を一つにまとめる方法」です。次の順に決めると、データ構造に渡す定義を作れます。

1. **範囲から何を返したいか決める。** 和なら和、最小値と個数ならその両方を状態 `value_type` にします。
2. **二つの範囲の答えを結合する `op(a,b)` を決める。** 左右の範囲の状態だけから結合後の答えが作れる必要があります。
3. **空範囲の答え `id()` を決める。** `op(id(), x) == op(x, id()) == x` を満たします。初期化時に「空」を意味するのか、値 0 の要素を意味するのかも区別します。
4. **演算の条件を確認する。** 一次元の通常セグメント木は結合則 `op(op(a,b),c) == op(a,op(b,c))` が必要です。二次元セグメント木と矩形集約 kd-tree はさらに可換則 `op(a,b) == op(b,a)` が必要です。逆元は必要ありません。
5. **値域と小さい例を試す。** 空範囲、一点、重複、極端な値、更新後の範囲を素朴計算と比較します。保持値と途中計算が型に収まる範囲を決めます。

| 目的 | `value_type` | `op(a,b)` | `id()` と注意 |
| --- | --- | --- | --- |
| 和 | 整数 | `a+b` | `0`。途中の和もオーバーフローしない |
| 最小値 | 整数 | `min(a,b)` | その型の最大値。最大値自体も入力できるが、値だけでは空と区別できない |
| 最大公約数 | 非負整数 | `gcd(a,b)` | `0`。`gcd(0,0)=0` |
| 最小値と個数 | `{最小値, 個数}` | 小さい方を選び、等しければ個数を足す | 個数 `0` を空とし、値が最大値でも区別する。個数の合計にも上限がある |

例えば `long long` の最小値用 Monoid は次のように定義します。`id()` は「空を表す印」であり、全ての実値より厳密に大きい値を作る必要はありません。

```cpp
#include <algorithm>
#include <limits>
struct Minimum {
  using value_type = long long;
  static value_type op(value_type a, value_type b) { return std::min(a, b); }
  static value_type id() { return std::numeric_limits<value_type>::max(); }
};
```

既製の `cp::Gcd<unsigned long long>`、`cp::MinCount<long long>`、`cp::RangeAggregate<long long, cp::AggregateKind::sum>` も利用できます。`RangeAggregate` は単なる整数ではなく `{value, length}` を保持するため、葉は `leaf(x)` で作り、結果は `.value` で読みます。二次元木に直接整数を入れたいだけなら、各ページの `Sum` のような単純な型を定義できます。

### データ構造に渡す

`cp::Segtree<Minimum>` なら一次元の点更新と区間最小値に使えます。二次元では、例えば `cp::SparseSegtree2D<long long, Minimum>` と指定し、更新対象の座標を登録してから `set(x,y,v)`、`prod(xl,xr,yl,yr)` を呼びます。[座標登録型の完成例](sparse_segtree_2d.md) と [オンライン型の完成例](dynamic_segtree_2d.md) は、同じインターフェースの `Sum` を使います。矩形は半開区間です。両者とも未設定の点は `id()` で、`set` は加算ではなく上書きです。

浮動小数点の加算は丸めにより結合則を厳密には満たさず、木の形で結果が変わり得ます。`Rational128` も中間値が範囲外になると例外が発生し、例外を投げる演算は、その実行範囲で通常の閉じた Monoid としては扱えません。二次元木の更新途中に例外が発生すると木の状態は復元されないため、入力範囲に注意し、例外後は再構築します。

### 遅延操作を追加する

区間一括更新では、結合する値に加え「何を適用するか」の定義が必要です。まず、一つの区間の状態だけで更新後の状態を計算できるか確認します。例えば区間和に一括加算するには、和のほかに区間長が必要です。次に、作用の適用と複数作用の合成、何もしない作用を定義します。結合との両立と合成順を具体例で確認してください。単なる区間和だけから一括 `chmin` 後の和を求めることはできません。

### 型のインターフェース

通常木用の型 `M` は `value_type`、`static value_type op(value_type, value_type)`、`static value_type id()` を用意します。遅延木用の型 `A` はさらに `action_type`、`mapping(action_type, value_type)`、`composition(action_type, action_type)`、`action_id()` を同じ値渡しの形で用意します。`leaf` と `range_values` はこのヘッダーの便宜機能で、アダプターそのものの必須要件ではありません。

例えば、点変更・区間 XOR は `Bitwise` を使えますが、独自定義を追加するなら次のように書けます。

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

追加候補には、文字列連結、最大部分和、区間代入と文字数カウント、modint による一次変換があります。どの作用が使えるかは保持する情報次第です。例えば単なる区間和だけから一括 `chmin` 後の和を求めることはできず、別のデータ構造や追加情報が必要です。非可換な演算では `op(left, right)` の順序を守り、逆順でも検証してください。

`RangeAggregate`・`RangeAdd`・`RangeAssign`・`RangeAffineSum` は符号付き整数型を想定し、長さと、結合・適用・作用の合成のすべての中間結果が型の範囲に収まることが前提です。オーバーフローは検出しません。`length` は `long long`、和の場合は `T` に変換されるので長さも `T` で表現可能にしてください。`int` の端点とメモリの制約は ACL に従います。一次変換には modular arithmetic が必要な場合が多く、標準整数の代わりに利用する型に応じた独自ポリシーを定義してください。

構築は `O(n)`、点操作・区間操作は `O(log n)`、メモリは `O(n)`（いずれも定義した演算が定数時間の場合）です。gcd は結合にユークリッドの互除法の計算時間がかかります。ローカル検証では区間加算・代入・一次変換、gcd・bitwise・最小値と回数を単純な配列と比較します。検証ページの「Verified with」から各テストを開けます。
