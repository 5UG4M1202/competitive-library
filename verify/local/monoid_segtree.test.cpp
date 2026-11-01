// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
// Intended correctness: compare all policies with a direct vector model.
#include "../../library/data_structure/monoid_segtree.hpp"

#include <algorithm>
#include <cassert>
#include <limits>
#include <random>
#include <vector>

struct Xor {
  using value_type = unsigned;
  static unsigned op(unsigned a, unsigned b) { return a ^ b; }
  static unsigned id() { return 0; }
};

template <cp::AggregateKind Kind, bool Assign>
void check_policy() {
  using P = std::conditional_t<Assign, cp::RangeAssign<long long, Kind>, cp::RangeAdd<long long, Kind>>;
  std::vector<long long> a(19);
  cp::LazySegtree<P> seg(cp::range_values<P>(a));
  std::mt19937 rng(8192 + static_cast<int>(Kind) * 10 + Assign);
  for (int i = 0; i < 1000; ++i) {
    int l = static_cast<int>(rng() % 20), r = static_cast<int>(rng() % 20);
    if (l > r) std::swap(l, r);
    long long x = static_cast<long long>(rng() % 21) - 10;
    if constexpr (Assign) seg.apply(l, r, typename P::action_type{x});
    else seg.apply(l, r, x);
    for (int j = l; j < r; ++j) {
      if constexpr (Assign) a[j] = x;
      else a[j] += x;
    }
    int ql = static_cast<int>(rng() % 20), qr = static_cast<int>(rng() % 20);
    if (ql > qr) std::swap(ql, qr);
    auto actual = seg.prod(ql, qr);
    assert(actual.length == qr - ql);
    if (ql == qr) assert(actual.value == P::id().value);
    else if constexpr (Kind == cp::AggregateKind::sum) {
      long long expected = 0;
      for (int j = ql; j < qr; ++j) expected += a[j];
      assert(actual.value == expected);
    } else if constexpr (Kind == cp::AggregateKind::minimum) {
      assert(actual.value == *std::min_element(a.begin() + ql, a.begin() + qr));
    } else {
      assert(actual.value == *std::max_element(a.begin() + ql, a.begin() + qr));
    }
    assert(seg.all_prod().length == static_cast<long long>(a.size()));
  }
  cp::LazySegtree<P> empty(0);
  empty.apply(0, 0, P::action_id());
  assert(empty.prod(0, 0).length == 0);
}

int main() {
  check_policy<cp::AggregateKind::sum, false>();
  check_policy<cp::AggregateKind::minimum, false>();
  check_policy<cp::AggregateKind::maximum, false>();
  check_policy<cp::AggregateKind::sum, true>();
  check_policy<cp::AggregateKind::minimum, true>();
  check_policy<cp::AggregateKind::maximum, true>();

  using A = cp::RangeAffineSum<long long>;
  std::vector<long long> a{1, 2, 3};
  cp::LazySegtree<A> seg(cp::range_values<A>(a));
  seg.apply(0, 3, {2, 1});
  seg.apply(1, 3, {3, 4});
  assert(seg.get(0).value == 3 && seg.get(1).value == 19 && seg.get(2).value == 25);
  assert(seg.prod(0, 3).value == 47);
  seg.apply(0, 2, {0, 5});
  assert(seg.all_prod().value == 35);
  assert(seg.max_right(0, [](A::value_type x) { return x.value <= 10; }) == 2);

  std::vector<long long> model(13);
  cp::LazySegtree<A> affine(cp::range_values<A>(model));
  std::mt19937 rng(5568);
  for (int i = 0; i < 1000; ++i) {
    int l = static_cast<int>(rng() % 14), r = static_cast<int>(rng() % 14);
    if (l > r) std::swap(l, r);
    long long mul = static_cast<long long>(rng() % 3) - 1;
    long long add = static_cast<long long>(rng() % 11) - 5;
    affine.apply(l, r, {mul, add});
    for (int j = l; j < r; ++j) model[j] = mul * model[j] + add;
    int ql = static_cast<int>(rng() % 14), qr = static_cast<int>(rng() % 14);
    if (ql > qr) std::swap(ql, qr);
    long long sum = 0;
    for (int j = ql; j < qr; ++j) sum += model[j];
    auto actual = affine.prod(ql, qr);
    assert(actual.value == sum && actual.length == qr - ql);
  }

  using MinAssign = cp::RangeAssign<long long, cp::AggregateKind::minimum>;
  cp::LazySegtree<MinAssign> extremes(cp::range_values<MinAssign>(std::vector<long long>{1}));
  extremes.apply(0, 1, std::numeric_limits<long long>::max());
  assert(extremes.get(0).value == std::numeric_limits<long long>::max());
  assert(extremes.get(0).length == 1);

  using M = cp::RangeAggregate<long long, cp::AggregateKind::sum>;
  cp::Segtree<M> plain(cp::range_values<M>(a));
  plain.set(1, M::leaf(7));
  assert(plain.prod(0, 3).value == 11);
  cp::Segtree<Xor> custom(std::vector<unsigned>{1, 2, 3});
  assert(custom.prod(0, 2) == 3);
  using Small = cp::RangeAffineSum<short>;
  cp::LazySegtree<Small> small(cp::range_values<Small>(std::vector<short>{1, 2}));
  small.apply(0, 2, {2, 1});
  assert(small.prod(0, 2).value == 8);
}
