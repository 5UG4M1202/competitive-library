---
title: 編集距離
documentation_of: ./levenstein_distance.hpp
---

# 編集距離

2つの文字列を、1バイトの挿入・削除・置換を1回の操作として変換する最小回数を求めます。関数名は従来 API の綴り `levenstein_distance` を維持しています。

```cpp
#include "library/string/levenstein_distance.hpp"
int d = levenstein_distance("kitten", "sitting"); // 3
```

空文字列も使えます。`std::string` のバイトを比較するため、UTF-8 の文字や書記素単位の距離ではありません。長さ、`n+1`、`m+1`、距離や中間値が `int` に収まり、二次元表を確保できることが必要です。入力長を `n, m` とすると、時間・追加メモリともに `O((n+1)(m+1))` です。長い文字列にはメモリを削減した別の方法を検討してください。

`cpp.json` の従来実装に標準ヘッダー、名前修飾、複数翻訳単位で使うための `inline` を追加して分離しました。従来部分の具体的な来歴は未確認です。Apache-2.0 は rrrriki の権利を持つ著作物性のある人による寄与に適用し、他者の権利を保証するものではありません。
