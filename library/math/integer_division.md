---
title: 整数除算の丸め
documentation_of: ./integer_division.hpp
---

# 整数除算の丸め

`ceil_div(a,b)` は実数の商を正の無限大方向、`floor_div(a,b)` は負の無限大方向、`out_div(a,b)` はゼロから離れる方向へ丸めます。整数の商であればそのまま返します。

```cpp
#include "library/math/integer_division.hpp"
auto a = ceil_div(-7, 3);  // -2
auto b = floor_div(-7, 3); // -3
auto c = out_div(-7, 3);   // -3
auto d = ceil_div(7, -3);  // -2
```

引数と結果は `long long`。除数は 0 以外で、`LLONG_MIN / -1` は結果が型に収まらないため使用できません。これらの前提を `assert` で検査しますが、`NDEBUG` 時も呼び出し側が守ってください。除数の符号を含めた全符号の組み合わせで使え、時間・追加空間は `O(1)` です。`constexpr` でも使用できます。

旧 `ceil_div` / `floor_div` は負の除数を扱えず、旧 `ceil_div` は加算でオーバーフローし得ました。修正後は商と剰余で丸めます。旧 `out_div` は [DNEK の記事](https://blog.dnek.net/entry/2020/12/06/011925)の実装と一致しますが、実際の取得経路と再利用条件は未確認です。詳細は第三者通知を参照してください。全符号と 64 ビット境界を広い整数型の不等式で検証しています。
