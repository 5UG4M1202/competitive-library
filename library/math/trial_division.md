---
title: 試し割りと素数表
documentation_of: ./trial_division.hpp
---

# 試し割りと素数表

前計算をせずに単発の整数を扱う場合に使います。指定上限以下の整数を多数処理する場合は、最小素因数表を構築する `PrimeUtil` も検討してください。

```cpp
#include "library/math/trial_division.hpp"
bool prime = is_prime(97);     // true
auto factors = prime_factor(60); // {2:2, 3:1, 5:1}
auto divisors = divisor(12);   // {1,2,3,4,6,12}
auto sieve = sieve_of_era(10); // sieve[2], sieve[3], sieve[5], sieve[7] が true
```

`is_prime(x)` は `x < 2` なら false。`prime_factor(n)` と `divisor(n)` の前提は `1 <= n <= LLONG_MAX` です。1 の素因数分解は空、正の約数は `{1}` です。0 の素因数分解や約数列挙は定義せず、負数にも対応しません。無効な引数は `assert` で検出しますが、`NDEBUG` 時も呼び出し側が前提を守ってください。

`sieve_of_era(n)` は `0 <= n < LLONG_MAX` が前提で、`0..n` の `n+1` 要素を返します。要素数が `size_t` とコンテナの上限に収まり、メモリを確保できる必要があります。`n=0` は `{false}` です。試し割り関数は最悪 `O(sqrt(n))` 時間、約数の整列にはさらに約数数 `d` に対し `O(d log d)`、ふるいは `O(n log log n)` 時間・`O(n)` ビットの表を要します。64 ビット上限近くの素数や大きな素因数の試し割りは現実的な時間で終わらない場合があります。

従来テンプレートから分離し、`prime_factor(0)` が停止しない問題、`sieve_of_era(0)` の範囲外アクセスと、ループ条件・更新のオーバーフローを修正しました。従来部分の具体的な来歴は未確認です。小さな整数の全約数・分解結果と、64 ビット境界を検証しています。
