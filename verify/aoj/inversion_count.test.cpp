// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/ALDS1_5_D
// competitive-verifier: DISPLAY no-index
#include "../../library/data_structure/inversion_count.hpp"
#include <iostream>
#include <vector>
int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int n;
  std::cin >> n;
  std::vector<int> a(static_cast<std::size_t>(n));
  for (auto& x : a) std::cin >> x;
  std::cout << cp::inversion_count(a) << '\n';
}
