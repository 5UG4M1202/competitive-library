---
title: ローリングハッシュ
documentation_of: ./rolling_hash.hpp
---

# ローリングハッシュ

文字列や数列の部分列を繰り返し比較するときのハッシュです。法は `2^61-1` で、ハッシュの一致は文字列の一致を保証しません。確実な一致が必要な場合は元の文字列も比較してください。長さの異なる列は、先頭のゼロなどにより特に衝突しやすいため、長さも一緒に比較します。

```cpp
#include "library/string/rolling_hash.hpp"
RollingHash rh;
auto h = rh.build(std::string("abracadabra"));
bool same = rh.query(h, 0, 3) == rh.query(h, 7, 10); // true: abr
int common = rh.lcp(h, 0, 11, h, 7, 11); // 4: abra
```

`build` は長さ `n+1` の累積表を返し、`query(h,l,r)` は半開区間 `[l,r)`、`combine(h1,h2,len2)` は先に `h1`、次に長さ `len2` の `h2` を連結します。`lcp` は指定した二つの半開区間の共通接頭辞の長さを返します。比較する表・ハッシュは**同じ base**で作ってください。既定の base はインスタンスごとに乱数生成されるため、別のインスタンスの値をそのまま比較できません。再現性が必要なら `RollingHash rh(1000003);` のように固定できますが、固定すると入力によって衝突が起きやすくなり得ます。

文字列は Unicode の文字ではなくバイト列として扱い、各バイトを `0..255` に変換します。符号付き `char` による従来の誤ったハッシュを修正したため、0x80 以上を含む文字列のハッシュ値は以前と変わります。`vector<T>` 版の各要素は `uint64_t` に変換可能で `0 <= 値 < mod` としてください。負数、`mod` 以上や複雑な型は事前に適切に符号化します。長さは `INT_MAX` 未満、区間は `0 <= l <= r <= n`、自分で指定する base は `0 < base < mod` が前提です。`add` と `mul` に渡す数も `[0,mod)` です。`__uint128_t` を使うため GCC・Clang など対応するコンパイラが必要です。

構築は時間・空間とも `O(n)`。`query` と `combine` は必要な base のべき乗を未計算なら追加するため、初回は追加分の時間・空間を使い、準備済みなら `O(1)`。`lcp` は長さ `m` に対し、べき乗の拡張を除いて `O(log m)` です。擬似乱数生成と単一ハッシュは暗号用途に適しません。

従来の実装は [ei1333/library の旧実装](https://competitive-verifier.github.io/zzz_ei1333_library/string/rolling-hash.hpp.html)と非常に近く、比較対象のリポジトリは [Unlicense](https://github.com/competitive-verifier/zzz_ei1333_library/blob/master/LICENSE) です。実際の取得経路は未確認です。[第三者通知](../../THIRD_PARTY_NOTICES.md)も参照してください。テストは空列、非 ASCII バイト、ランダムな部分列・結合・LCP を検証しますが、衝突が起きないことは保証しません。
