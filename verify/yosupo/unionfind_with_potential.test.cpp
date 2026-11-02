// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/unionfind_with_potential
// competitive-verifier: DISPLAY no-index
// Checks modular differences, consistent merges and rejected contradictions.
#include "../../library/data_structure/union_find_with_potential.hpp"
#include <atcoder/modint>
#include <iostream>

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int n, q;
  std::cin >> n >> q;
  UnionFindWithPotential<atcoder::modint998244353> uf(n);
  while (q--) {
    int t, u, v;
    std::cin >> t >> u >> v;
    if (t == 0) {
      int x;
      std::cin >> x;
      std::cout << uf.merge(u, v, x) << '\n';
    } else if (uf.same(u, v)) {
      std::cout << uf.diff(u, v).val() << '\n';
    } else {
      std::cout << -1 << '\n';
    }
  }
}
