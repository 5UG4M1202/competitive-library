---
title: UnionFindWithPotential
documentation_of: ./union_find_with_potential.hpp
---

# UnionFindWithPotential

重み付き Union-Find。`merge(x, y, p)` は `P(x) - P(y) = p` を追加し、既存の制約と矛盾すれば `false` を返します。連結済みの2頂点に対して `diff(x, y)` は `P(x) - P(y)` を返します。

```cpp
#include "library/data_structure/union_find_with_potential.hpp"
UnionFindWithPotential<long long> uf(3);
uf.merge(0, 1, 4);  // P(0) - P(1) = 4
uf.merge(1, 2, 3);  // P(1) - P(2) = 3
// uf.diff(0, 2) == 7
```

頂点は `0 <= x, y < N`、`N >= 0` が前提です。`diff` は同じ連結成分の頂点間でのみ意味を持ちます。`potential(x)` は現在の根からの相対値で、異なる成分間や merge の前後で直接比較できません。`dat` と `pot` は互換性のため公開されていますが、直接変更すると不変条件が壊れます。

`T()` が加法の単位元で、加算、減算、単項マイナス、等値比較を持つ可換な加法群を想定します。すべての中間計算が `T` に収まることが必要です。整数のオーバーフローや浮動小数点の丸めを検出しません。添字の範囲検査も行いません。経路圧縮があるため、参照系の操作も内部状態を変更します。

メモリは `O(N)`、初期化は `O(N)`、各操作は重み付き併合と経路圧縮により償却 `O(α(N))` です（`T` の演算を定数時間と仮定）。

実装は [NyaanNyaan/library](https://github.com/NyaanNyaan/library/blob/b3981adc80a800b2584980b01821324ea6c77183/data-structure/union-find-with-potential.hpp) の CC0-1.0 実装に標準ヘッダーと名前修飾を補ったものです。詳細は [第三者通知](../../THIRD_PARTY_NOTICES.md) を参照してください。
