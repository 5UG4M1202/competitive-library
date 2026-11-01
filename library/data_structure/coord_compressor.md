---
title: 座標圧縮
documentation_of: ./coord_compressor.hpp
---

# 座標圧縮

複数の配列に含まれる値をまとめてソート・重複除去し、小さい順に `0, 1, ...` の添字へ変換します。圧縮した値を元に戻す必要がある場合にも使えます。

```cpp
#include "library/data_structure/coord_compressor.hpp"
std::vector<int> a{30, 10, 30}, b{20};
CoordCompressor<int> cc;
auto [ca, cb] = cc.build(a, b); // ca={2,0,2}, cb={1}
int original = cc.decompress(1); // 20
```

`build` は複数の非 const lvalue 配列を受け取り、戻り値の tuple に同じ順で圧縮後配列を返します。`build2D` は二次元 `vector` をまとめて圧縮します。`compress` と `decompress`、および対応する vector 版は、その build で登録済みの値・添字にだけ使ってください。未登録値は `assert` の失敗、または `assert` を無効にした場合 `unordered_map::at` の例外となります。

`build` / `build2D` を再度呼ぶと、以前の対応を破棄して新しい入力から作り直します。以前の build で得た圧縮値は新しい対応では使えません。従来実装は古い対応が残る不具合があり、この動作を修正しました。

型 `T` は `std::hash<T>` と等値比較、sort の順序比較を持ち、順序による等価性と等値比較が整合することが必要です。またコピー・代入・既定構築が必要です。異なる値の数とインデックスが `int` に収まり、全要素とハッシュ表を確保できることが必要です。範囲端点の補完は行いません。

入力の総要素数を `N`、異なる値を `K` とすると構築は sort と平均的なハッシュ操作で `O(N log N)`、追加メモリ `O(N + K)`、単一変換はハッシュ操作の平均 `O(1)` です。ハッシュ表の最悪計算量は衝突状況に依存します。

従来 snippet から標準ヘッダーと名前修飾を加えて分離しました。具体的な来歴は未確認です。Apache-2.0 は rrrriki の権利を持つ著作物性のある人による寄与に適用し、他者の権利を保証するものではありません。
