---
title: DistinctKSegments
documentation_of: //library/data_structure/distinct_k_segments.hpp
---

# DistinctKSegments

列を `K` 個の空でない連続区間に分割し、各区間に含まれる異なる値の個数の合計を最大化します。

```cpp
#include <iostream>
#include <vector>
#include "library/data_structure/distinct_k_segments.hpp"
int main() {
  DistinctKSegments solver(std::vector<int>{0, 1, 0}, 2);
  std::cout << solver.solve() << '\n'; // 3
}
```

AtCoder Library の `<atcoder/lazysegtree>` が必要です。リポジトリルートと ACL の include path を指定し、C++17 以上でコンパイルしてください。

## 前提と操作

- `DistinctKSegments<Int>(a, K).solve()` は最大値を返します。空でない列では `K` を `[1, N]` に丸め、空列では `K = 0`、結果は `0` です。
- 値は `int` で表せる非負整数とし、メモリ節約のため `0..M-1` への座標圧縮を推奨します。`N + 1` と最大値 `+ 1` も `int` で表現でき、必要なメモリを確保できることが前提です。
- 内部の区間は半開区間です。集約は非負値を前提に単位元 `0` を使います。集計は `long long` を使い、オーバーフローは検出しません。
- `solve()` は内部状態を変更するため、1インスタンスにつき1回だけ呼んでください。再実行時は状態が初期化されず、結果を保証しません。

計算量は `O(K N log N)`、メモリは `O(K N + M)` です。`M` は最大値に1を足した大きさです。`verify/local/distinct_k_segments.test.cpp` は境界と素朴な分割との決定的ランダム比較を行います。

## 来歴とライセンス

ユーザーは元の `@DistinctMax` を AI の補助を使って作成したと申告しています。元スニペットの参考提出 <https://atcoder.jp/contests/abc397/submissions/71461138> は保持しており、このリンク自体がライセンスを証明するものではありません。Apache-2.0 は rrrriki の著作物性のある人による寄与に適用され、この申告だけで生成断片の権利が保証されるわけではありません。ACL は外部の CC0 依存です。
