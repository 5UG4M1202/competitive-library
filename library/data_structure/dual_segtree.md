---
title: 双対セグメント木
documentation_of: ./dual_segtree.hpp
---

# 双対セグメント木

区間への操作を重ね、各位置に累積した操作を取り出します。区間の合計や最小値は取得できません。区間集約も必要なら Monoid 対応のセグメント木や遅延セグメント木を検討してください。

```cpp
#include "library/data_structure/dual_segtree.hpp"
#include <vector>
struct Affine { long long a, b; }; // x -> a*x + b
Affine compose(Affine f, Affine g) { // 先に g、次に f
  return {f.a * g.a, f.a * g.b + f.b};
}
Affine identity() { return {1, 0}; }

std::vector<Affine> initial(3, {0, 5}); // 各値を定数関数として表す
dual_segtree<Affine, compose, identity> seg(initial);
seg.apply(0, 3, {2, 1}); // 値を 2*x+1 に
seg.apply(1, 2, {0, 7}); // 位置1を 7 に代入
auto x = seg.get(0).b; // 11
auto y = seg.get(1).b; // 7
```

`composition(f,g)` は先に `g`、後に `f` を適用した作用を表し、結合則と `id()` の左右単位元が必要です。可換でなくても使えます。`dual_segtree(n)` は各位置を `id()` で、`dual_segtree(v)` は各位置を `v[i]` で初期化します。**初期値と作用は同じ型 `F`** のため、数値への代入などは例の定数関数のように表現するか、取得した作用を別に保持した初期値へ適用してください。

`apply(l,r,f)` は半開区間 `[l,r)`、`get(p)` は一点です。`0 <= l <= r <= n`、`0 <= p < n`、`0 <= n <= 2^29` を満たし、確保する `O(n)` のメモリと作用の演算結果が型に収まる必要があります。空区間への更新とサイズ 0 は使用できます。初期化は `O(n)`、更新と取得は作用の合成が `O(1)` のとき `O(log(n+1))` です。

境界の遅延伝播処理は [ACL の lazy_segtree v1.6](https://github.com/atcoder/ac-library/blob/v1.6/atcoder/lazysegtree.hpp) と非常に似ており、比較版は CC0 です。ただし実際の来歴は未確認です。従来のグローバルな補助関数 `ceil_pow2` は、ヘッダーでは名前の衝突を避けて `dual_segtree_ceil_pow2` にしています。非可換な一次変換を配列とランダム比較して検証しています。
