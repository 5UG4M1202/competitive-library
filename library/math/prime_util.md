---
title: PrimeUtil
documentation_of: ./prime_util.hpp
---

# PrimeUtil

指定上限までの最小素因数表を作り、繰り返し素数判定・素因数分解・約数列挙を行います。1回だけ大きな整数を処理する用途より、同じ上限以下の整数を多数処理する場合に向きます。

```cpp
#include "library/math/prime_util.hpp"
PrimeUtil primes(100);
auto factors = primes.factor(60); // {2:2, 3:1, 5:1}
auto divisors = primes.divisor(12); // {1,2,3,4,6,12}
bool yes = primes.is_prime(97);
```

通常の前提は `1 <= n`、`1 <= x <= n` です。`is_prime(x)` は `x < 2` で `false`、`factor(x)` は `x < 2` で空、`divisor(x)` は `x < 2` でも `{1}` を返す従来挙動です。0・負数の数学的な素因数分解や約数列挙として使わないでください。`factor` / `divisor` は上限超過を検査せず、範囲外アクセスになります。`is_prime` の上限検査は `assert` のため、無効化されたビルドでも呼び出し側が範囲を守ってください。

実装は `int` の `n+1`、`i*i`、ふるい内の `j+i` を用います。これらを含む全中間結果が `int` の範囲内であること、および確保可能なメモリ量であることが必要です。出力約数は `int` で表現されます。`divisor(x, false)` の順序は保証しません。

構築は `O(n log log n)`、メモリは `O(n)`、素数判定は `O(1)`、素因数分解は因子数と `std::map` 更新に依存し `O(log x * log log x)` を上限の目安とします。約数列挙は約数の個数 `d` に比例して生成し、整列する場合は `O(d log d)` を要します。

利用者が生成 AI を使って作成した従来実装から、単独 include 用の標準ヘッダーと名前修飾を加えて分離しました。PrimeUtil は Apache-2.0 で提供します。適用範囲は `LICENSE-SCOPE.md` を参照してください。
