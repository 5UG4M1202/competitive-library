// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
// Intended correctness: compare point updates and range queries with a vector.
#include "../../library/data_structure/monoid_segtree.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>
#include <numeric>
#include <random>
#include <vector>

template <class M, cp::BitwiseKind Kind>
void check_bitwise(std::mt19937& rng) {
  using U = typename M::value_type;
  std::vector<U> values(31);
  for (auto& x : values) x = static_cast<U>(rng());
  cp::Segtree<M> seg(values);
  for (int i = 0; i < 2000; ++i) {
    int p = static_cast<int>(rng() % values.size());
    U v = static_cast<U>(rng());
    values[p] = v;
    seg.set(p, v);
    int l = static_cast<int>(rng() % 32), r = static_cast<int>(rng() % 32);
    if (l > r) std::swap(l, r);
    U expected = M::id();
    for (int j = l; j < r; ++j) {
      if constexpr (Kind == cp::BitwiseKind::bit_and) expected = static_cast<U>(expected & values[j]);
      else if constexpr (Kind == cp::BitwiseKind::bit_or) expected = static_cast<U>(expected | values[j]);
      else expected = static_cast<U>(expected ^ values[j]);
    }
    assert(seg.prod(l, r) == expected);
  }
  const U a = static_cast<U>(rng()), b = static_cast<U>(rng()), c = static_cast<U>(rng());
  assert(M::op(M::op(a, b), c) == M::op(a, M::op(b, c)));
  assert(M::op(a, M::id()) == a && M::op(M::id(), a) == a);
}

int main() {
  using G = cp::Gcd<std::uint64_t>;
  static_assert(std::numeric_limits<std::uint64_t>::max() > 0);
  assert(G::op(0, 0) == 0);
  assert(G::op(std::numeric_limits<std::uint64_t>::max(), 0) == std::numeric_limits<std::uint64_t>::max());
  std::mt19937 rng(20261102);
  std::vector<std::uint64_t> values(29);
  cp::Segtree<G> gcd(values);
  for (int i = 0; i < 2000; ++i) {
    int p = static_cast<int>(rng() % values.size());
    values[p] = rng() % 10000;
    gcd.set(p, values[p]);
    int l = static_cast<int>(rng() % 30), r = static_cast<int>(rng() % 30);
    if (l > r) std::swap(l, r);
    std::uint64_t expected = 0;
    for (int j = l; j < r; ++j) expected = std::gcd(expected, values[j]);
    assert(gcd.prod(l, r) == expected);
    auto a = static_cast<std::uint64_t>(rng());
    auto b = static_cast<std::uint64_t>(rng());
    auto c = static_cast<std::uint64_t>(rng());
    assert(G::op(G::op(a, b), c) == G::op(a, G::op(b, c)));
    assert(G::op(a, G::id()) == a && G::op(G::id(), a) == a);
  }
  using And = cp::Bitwise<std::uint8_t, cp::BitwiseKind::bit_and>;
  using Or = cp::Bitwise<std::uint64_t, cp::BitwiseKind::bit_or>;
  using Xor = cp::Bitwise<std::uint64_t, cp::BitwiseKind::bit_xor>;
  assert(And::id() == std::numeric_limits<std::uint8_t>::max());
  check_bitwise<And, cp::BitwiseKind::bit_and>(rng);
  check_bitwise<Or, cp::BitwiseKind::bit_or>(rng);
  check_bitwise<Xor, cp::BitwiseKind::bit_xor>(rng);
  cp::Segtree<Or> bits(std::vector<std::uint64_t>{1, 2, 4, 8});
  assert(bits.max_right(0, [](std::uint64_t x) { return (x & 4) == 0; }) == 2);
  assert(bits.min_left(4, [](std::uint64_t x) { return (x & 2) == 0; }) == 2);

  using M = cp::MinCount<long long>;
  const auto max = std::numeric_limits<long long>::max();
  std::vector<long long> model(23, max);
  cp::Segtree<M> minima(cp::range_values<M>(model));
  assert(minima.prod(0, 0).count == 0);
  assert(minima.all_prod().value == max && minima.all_prod().count == 23);
  for (int i = 0; i < 2000; ++i) {
    int p = static_cast<int>(rng() % model.size());
    model[p] = static_cast<long long>(rng() % 9) - 4;
    minima.set(p, M::leaf(model[p]));
    int l = static_cast<int>(rng() % 24), r = static_cast<int>(rng() % 24);
    if (l > r) std::swap(l, r);
    auto actual = minima.prod(l, r);
    if (l == r) {
      assert(actual.count == 0 && actual.value == max);
    } else {
      auto minimum = *std::min_element(model.begin() + l, model.begin() + r);
      assert(actual.value == minimum);
      assert(actual.count == std::count(model.begin() + l, model.begin() + r, minimum));
    }
    auto a = M::leaf(model[rng() % model.size()]);
    auto b = M::leaf(model[rng() % model.size()]);
    auto c = M::leaf(model[rng() % model.size()]);
    auto left = M::op(M::op(a, b), c), right = M::op(a, M::op(b, c));
    assert(left.value == right.value && left.count == right.count);
    assert(M::op(a, M::id()).value == a.value && M::op(M::id(), a).count == a.count);
  }
}
