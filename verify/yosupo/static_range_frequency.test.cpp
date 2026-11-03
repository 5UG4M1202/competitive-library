// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/static_range_frequency
// competitive-verifier: DISPLAY no-index
#include "../../library/data_structure/wavelet_matrix.hpp"
#include <iostream>
#include <vector>
int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::size_t n, q;
  std::cin >> n >> q;
  std::vector<int> values(n);
  for (auto& value : values) std::cin >> value;
  cp::WaveletMatrix<int> wm(values);
  while (q--) {
    std::size_t l, r;
    int value;
    std::cin >> l >> r >> value;
    std::cout << wm.count(l, r, value) << '\n';
  }
}
