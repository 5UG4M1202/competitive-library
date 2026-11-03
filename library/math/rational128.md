---
title: 128 ビット整数の分数
documentation_of: ./rational128.hpp
---

# 128 ビット整数の分数

`cp::Rational128` は `__int128_t` の分子と正の分母を使う有限の有理数です。常に約分し、ゼロは `0/1` として保持します。分母は `1` 以上 `2^127-1` 以下、分子は `-2^127` 以上 `2^127-1` 以下です。GCC/Clang の拡張と C++20 を使い、MSVC には対応しません。ヘッダー自体に Boost などの外部依存はありません。

```cpp
#include "library/math/rational128.hpp"
#include <cassert>
#include <sstream>
int main() {
  using cp::Rational128;
  Rational128 a(6, 8), b(-1, 6);
  assert(a + b == Rational128(7, 12));
  assert(a * b == Rational128(-1, 8));
  assert(a > b && b.floor() == -1 && b.ceil() == 0);
  std::ostringstream out;
  out << a; // "3/4"
  assert(out.str() == "3/4");
}
```

コンストラクターは分子と分母を受け取り、分母を省略すれば `1` です。符号付き128ビットの最小値も受け付け、負の分母は符号を分子に移します。ただし約分後の分母・分子が上記の範囲に収まらなければ例外になります。`numerator()` と `denominator()` が値を返し、正規化状態を外から変更できません。

`+ - * /`、複合代入、単項 `-`、比較、`reciprocal()`、`sign()`、`trunc()`・`floor()`・`ceil()` を用意します。整数からの暗黙変換により `a + 2` も使えます。比較は交差積を作らず、分子・分母が表現範囲内なら比較途中の整数オーバーフローを起こしません。`static_cast<long double>(a)` は近似値で、丸めが生じます。浮動小数点からの直接構築は不可です。

ゼロ除算・分母ゼロは `std::domain_error`、範囲外は `std::overflow_error` を送出します。乗除では先に約分し、加減算では分母の共通因子を利用します。ただし **加減算の計算途中の積や和も signed 128 ビットに収まる必要があります**。最終結果が表現可能でも途中が収まらなければ例外です。例えば `max/2 + max/2` は数学的には `max` ですが、この実装では途中の和がオーバーフローするため例外になります。結果だけ収まれば常に計算できる型ではありません。例外後、複合代入の左辺は変更されません。

`str()` と `operator<<` は整数なら `-3`、分数なら `-3/4` を出力します。`parse()` と `operator>>` は、整数または空白を含まない `整数/整数` を受け付け、各整数に先頭の `+`・`-` を付けられます。十進小数・指数・空白を挟んだ分数には対応しません。`parse()` は構文不正で `std::invalid_argument`、分母ゼロと範囲外は上記の例外を送出します。ストリーム入力失敗時は `failbit` を立て、対象値を変更しません。

演算は約分にユークリッドの互除法を使い、比較は連分数で `O(log M)` 回の整数除算を行います（`M` は入力成分の絶対値の最大値）。保持メモリは定数です。例外を使うため `-fno-exceptions` では利用できません。

[Boost.Rational](https://www.boost.org/doc/libs/latest/libs/rational/rational.html) も選択肢で、この環境では `boost::rational<__int128_t>` の基本的な演算を確認しています。`Rational128` は外部依存なしの利用、明示的な範囲検査、128 ビット値の文字列入出力を提供します。有限の128ビット幅を超える結果や計算途中も確実に扱う場合は [Boost.Multiprecision の `cpp_rational`](https://www.boost.org/doc/libs/latest/libs/multiprecision/doc/html/boost_multiprecision/tut/rational/cpp_rational.html) を検討してください。

ローカルでは最小・最大値、分母ゼロ、計算途中のオーバーフロー、入出力、約分、比較の境界を検証し、固定 seed のランダム入力を Boost.Multiprecision の整数演算と照合します。Boost はこのテストの参照計算にのみ使用します。
