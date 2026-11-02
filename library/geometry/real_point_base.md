---
title: 実数点の基本型
documentation_of: ./real_point_base.hpp
---

# 実数点の基本型

`RealPoint` は `Point<long double, 2>` の別名です。`{x,y}` で初期化し、`x`, `y` と `operator[]` を使えます。加減算・通常の比較は EPS を適用しません。計算結果は丸め・オーバーフローがあり得ます。操作は定数時間・定数メモリです。幾何演算は実数点と偏角を参照してください。
