// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/range_affine_range_sum
// competitive-verifier: DISPLAY no-index
// Checks dynamic lazy propagation with affine actions and interval sums.
#include "../../library/data_structure/dynamic_lazy_segtree.hpp"
#include <atcoder/modint>
#include <iostream>
#include <vector>

using Mint = atcoder::modint998244353;
struct S { Mint sum, length; };
struct F {
  Mint a, b;
  bool operator==(const F&) const = default;
};
S op(S x, S y) { return {x.sum + y.sum, x.length + y.length}; }
S e() { return {0, 0}; }
S mapping(F f, S x) { return {f.a * x.sum + f.b * x.length, x.length}; }
F composition(F f, F g) { return {f.a * g.a, f.a * g.b + f.b}; }
F id() { return {1, 0}; }

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int n, q;
  std::cin >> n >> q;
  std::vector<Mint> prefix(n + 1);
  for (int i = 0; i < n; ++i) {
    int x;
    std::cin >> x;
    prefix[i + 1] = prefix[i] + x;
  }
  dynamic_lazy_segtree<S, op, e, F, mapping, composition, id> seg(
      0, n, [&](long long l, long long r) -> S {
        return {prefix[static_cast<std::size_t>(r)] - prefix[static_cast<std::size_t>(l)], r - l};
      });
  while (q--) {
    int t, l, r;
    std::cin >> t >> l >> r;
    if (t == 0) {
      int b, c;
      std::cin >> b >> c;
      seg.apply(l, r, {b, c});
    } else {
      std::cout << seg.prod(l, r).sum.val() << '\n';
    }
  }
}
