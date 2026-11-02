// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/range_affine_point_get
// competitive-verifier: DISPLAY no-index
// Checks range updates whose affine actions do not commute.
#include "../../library/data_structure/dual_segtree.hpp"
#include <atcoder/modint>
#include <iostream>
#include <vector>

using Mint = atcoder::modint998244353;
struct Affine { Mint a, b; };
Affine composition(Affine f, Affine g) { return {f.a * g.a, f.a * g.b + f.b}; }
Affine id() { return {1, 0}; }

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int n, q;
  std::cin >> n >> q;
  std::vector<Affine> initial(n);
  for (auto& f : initial) {
    int x;
    std::cin >> x;
    f = {0, x};
  }
  dual_segtree<Affine, composition, id> seg(initial);
  while (q--) {
    int t;
    std::cin >> t;
    if (t == 0) {
      int l, r, b, c;
      std::cin >> l >> r >> b >> c;
      seg.apply(l, r, {b, c});
    } else {
      int p;
      std::cin >> p;
      std::cout << seg.get(p).b.val() << '\n';
    }
  }
}
