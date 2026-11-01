---
title: 行列の90度回転
documentation_of: ./rotate_matrix.hpp
---

# 行列の90度回転

長方形の二次元配列、または同じ長さの文字列からなるグリッドを時計回りに90度回転します。

```cpp
#include "library/misc/rotate_matrix.hpp"
std::vector<std::string> grid{"abc", "def"};
auto rotated = rotate_matrix(grid); // {"da", "eb", "fc"}
```

入力は少なくとも1行があり、全行の長さが同じである必要があります。0行や不揃いな行は範囲外アクセスになるため使用できません。列数が0のときは空の結果を返します。`std::string` 版はバイト単位で扱い、UTF-8 の文字単位回転には対応しません。行数と列数は `int` に収まり、結果を保持できるメモリが必要です。

行数を `H`、列数を `W` とすると、時間 `O(HW)`、追加メモリ `O(HW)` です。値型は結果の既定構築と要素代入が可能であることが必要です。

`cpp.json` の従来の2 overload を単独 include 可能にしました。従来部分の具体的な来歴は未確認です。Apache-2.0 は rrrriki の権利を持つ著作物性のある人による寄与に適用し、他者の権利を保証するものではありません。
