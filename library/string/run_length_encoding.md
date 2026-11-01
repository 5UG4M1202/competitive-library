---
title: ランレングス圧縮
documentation_of: ./run_length_encoding.hpp
---

# ランレングス圧縮

連続する同じ値を `(値, 個数)` にまとめます。`std::string` と `std::vector<T>` の overload があります。

```cpp
#include "library/string/run_length_encoding.hpp"
auto runs = rle(std::string("aaabbc")); // {('a',3), ('b',2), ('c',1)}
```

空の入力には空の配列を返します。文字列は Unicode の文字ではなくバイト単位です。`T` にはコピーと不等比較が必要です。入力長は `INT_MAX` 以下を前提とし、時間・追加空間はともに `O(n)` です。従来の両テンプレートでは空入力の末尾を参照していたため修正しました。

従来テンプレートから分離した実装で、具体的な来歴は未確認です。空入力とランダムな配列からの復元を検査しています。
