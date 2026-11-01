// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/data_structure/dual_segtree.hpp"
#include <cassert>
#include <random>
#include <vector>

constexpr long long mod = 1000000007;
struct Affine { long long a, b; };
Affine composition(Affine f, Affine g) {
  return {f.a * g.a % mod, (f.a * g.b + f.b) % mod};
}
Affine id() { return {1, 0}; }

int main() {
  dual_segtree<Affine, composition, id> empty(0);
  empty.apply(0, 0, {0, 4});
  dual_segtree<Affine, composition, id> order(3);
  order.apply(0, 3, {2, 1});
  order.apply(1, 2, {3, 4});
  auto value = order.get(1);
  assert(value.a == 6 && value.b == 7);
  std::mt19937 rng(314159);
  for (int n = 1; n <= 65; ++n) {
    std::vector<Affine> model(n);
    for (auto &x : model) x = {static_cast<long long>(rng() % 5), static_cast<long long>(rng() % 9)};
    dual_segtree<Affine, composition, id> seg(model);
    for (int iter = 0; iter < 1000; ++iter) {
      int l = static_cast<int>(rng() % (n + 1));
      int r = static_cast<int>(rng() % (n + 1));
      if (l > r) std::swap(l, r);
      Affine f{static_cast<long long>(rng() % 4), static_cast<long long>(rng() % 7)};
      seg.apply(l, r, f);
      for (int i = l; i < r; ++i) model[i] = composition(f, model[i]);
      for (int j = 0; j < 3; ++j) {
        int p = static_cast<int>(rng() % n);
        auto got = seg.get(p);
        assert(got.a == model[p].a && got.b == model[p].b);
      }
    }
    for (int i = 0; i < n; ++i) {
      auto got = seg.get(i);
      assert(got.a == model[i].a && got.b == model[i].b);
    }
  }
}
