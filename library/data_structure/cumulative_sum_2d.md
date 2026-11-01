---
title: 二次元累積和
documentation_of: ./cumulative_sum_2d.hpp
---

# 二次元累積和

固定した格子の矩形和を繰り返し求めます。点への加算をすべて終えてから1回 `build()` し、その後に矩形和を問い合わせます。更新と問い合わせを交互に行う場合には適しません。

```cpp
#include "library/data_structure/cumulative_sum_2d.hpp"
CumulativeSum2D<long long> grid(2, 3); // 高さ2、幅3
grid.add(2, 0, 5);                     // x=2, y=0
grid.add(1, 1, 3);
grid.build();
auto sum = grid.query(1, 0, 3, 2);    // [1,3) × [0,2) の和 = 8
```

コンストラクタは `(H, W)`、`add` は `(x, y, 値)`、`query` は `(sx, sy, gx, gy)` の順です。矩形は `[sx, gx) × [sy, gy)` で、右端と下端を含みません。`0 <= H, W`、`0 <= x < W`、`0 <= y < H`、`0 <= sx <= gx <= W`、`0 <= sy <= gy <= H` が前提です。空矩形は `0` を返します。高さまたは幅が0のときも空矩形を問い合わせられます。

`add` は上限以上の座標を無視する従来挙動ですが、負の座標を含め、範囲外を入力として使わないでください。`build` を複数回呼ぶことや build 後に `add` することは想定されず、これらを検知しません。`T` は `0` から構築でき、加算・減算・加算代入を持つ必要があります。`H+1`、`W+1` が `int` に収まり、全格子と、累積和および計算途中の値が型・メモリに収まることが必要です。

構築と `build` は `O(HW)`、`add` と `query` は `O(1)`、メモリは `O(HW)` です。空間計算量を抑えたい場合は、入力の制約と使用頻度を考えて別の方法を選んでください。

従来の `cpptemplate` から分離しました。[ei1333/library の実装](https://github.com/ei1333/library/blob/088b4c8b49f85194099305d998e7113dc3d5dd2e/dp/cumulative-sum-2d.hpp)とよく似ていますが、添字の順序が異なります。比較した上流版は [Unlicense](https://github.com/ei1333/library/blob/088b4c8b49f85194099305d998e7113dc3d5dd2e/LICENSE) です。実際の来歴を断定せず、詳細は [第三者通知](../../THIRD_PARTY_NOTICES.md) を参照してください。
