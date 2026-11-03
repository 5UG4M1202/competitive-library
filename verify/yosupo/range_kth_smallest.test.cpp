// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/range_kth_smallest
// competitive-verifier: DISPLAY no-index
#include "../../library/data_structure/wavelet_matrix.hpp"
#include <iostream>
#include <vector>
int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int n,q; std::cin >> n >> q;
  std::vector<int> a(static_cast<std::size_t>(n));
  for (auto& x:a) std::cin >> x;
  cp::WaveletMatrix<int> wm(a);
  while (q--) { std::size_t l,r,k; std::cin >> l >> r >> k; std::cout << wm.kth(l,r,k) << '\n'; }
}
