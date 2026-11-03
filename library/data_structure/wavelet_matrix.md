---
title: Wavelet Matrix
documentation_of: ./wavelet_matrix.hpp
---

# Wavelet Matrix

`cp::WaveletMatrix<T>` は固定された整数列に対し、区間内の順位・頻度・値域内個数を求めます。`T` は標準整数型、負数・重複を扱い、値を座標圧縮します。構築後の値の変更はできません。

検証ページの「提出用の利用例」は本体と `main()` を一括コピーでき、C++20 で使えます。外部ライブラリは不要です。`main()` は問題の入出力に合わせて変更します。

```cpp
#include "library/data_structure/wavelet_matrix.hpp"
#include <cassert>
#include <vector>
int main() {
  cp::WaveletMatrix<int> wm(std::vector<int>{5, 1, 5, -2, 3});
  assert(wm.kth(1, 5, 1) == 1); // [-2,1,3,5] の 0 始まり 1 番目
  assert(wm.count(0, 5, 5) == 2);
  assert(wm.range_count(0, 5, 1, 5) == 2); // 1 以上 5 未満
  assert(wm.predecessor(0, 5, 1).value() == -2);
  assert(wm.lower_bound(0, 5, 2).value() == 3);
}
```

全操作で添字は `0 <= l <= r <= size()`、対象は半開 `[l,r)` です。`kth(l,r,k)` は `0 <= k < r-l` が必要です。`count(l,r,x)` は x の個数、`count_less(l,r,x)` は x 未満の個数、`range_count(l,r,lo,hi)` は `[lo,hi)` の値の個数です。`lo >= hi` と空区間は 0 です。`predecessor` は x 未満の最大値、`lower_bound` は x 以上の最小値を `optional` で返し、存在しない場合は空です。最大・最小値付近でも値に 1 を足しません。

長さ `N`、異なる値の数 `D` に対し、構築 `O(N log N + N log(D+1))`、各クエリ `O(log(D+1))`。各段は 64 ビットごとにビットと累積個数を保存するため、空間は `O((N/64+1) log(D+1)+D)` **語**です。構築中は `O(N)` 語の作業領域も使います。値・添字の比較とコピーが定数時間と仮定します。頻度の計算には加減算の範囲条件を要しません。空列も構築できます。更新が必要なら別の動的データ構造を検討してください。

参考測定: GCC 11.4 `-O2 -DNDEBUG`・WSL2、20 万要素と 20 万個の `kth`、固定 seed の一様乱数で、ビット圧縮前は構築 46 ms・質問 193 ms・最大 RSS 約 40.7 MiB、圧縮後は 43 ms・60 ms・約 13.9 MiB でした。各 1 回のプロセス全体の測定で、実行環境や入力により変わります。再測定には `tools/benchmark_wavelet_matrix.cpp` を使用できます。

`count` は該当する値の経路を一度だけたどります。別の参考測定では、GCC 11.4 `-O2 -DNDEBUG`・WSL2、20 万要素・100 万クエリで、旧方式 495 ms、新方式 271 ms でした。各 1 回で結果の合計が一致しています。入力分布や環境により変わり、再測定には `tools/benchmark_wavelet_count.cpp` を利用できます。

重複、負値、空区間、整数型の端、全区間の順位と頻度をソートや全走査と比較します。`kth` は [Library Checker: Range Kth Smallest](https://judge.yosupo.jp/problem/range_kth_smallest)、`count` は [Static Range Frequency](https://judge.yosupo.jp/problem/static_range_frequency) でも検証します。
