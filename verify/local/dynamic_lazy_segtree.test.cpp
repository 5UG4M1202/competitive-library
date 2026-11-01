// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
// Intended correctness and ownership of split dynamic_lazy_segtree.
#include "../../library/data_structure/dynamic_lazy_segtree.hpp"
#include <algorithm>
#include <cassert>
#include <random>
#include <type_traits>
#include <utility>
#include <vector>
using ll = long long;

struct S { long long sum, len; };
S op(S a, S b) { return {a.sum + b.sum, a.len + b.len}; }
S e() { return {0, 0}; }
using F = long long;
S mapping(F f, S x) { return {x.sum + f * x.len, x.len}; }
F composition(F f, F g) { return f + g; }
F id() { return 0; }
using Tree = dynamic_lazy_segtree<S, op, e, F, mapping, composition, id>;
struct Counted {
  static inline int live = 0;
  long long value;
  Counted(long long v = 0) : value(v) { ++live; }
  Counted(const Counted& x) : value(x.value) { ++live; }
  Counted(Counted&& x) noexcept : value(x.value) { ++live; }
  Counted& operator=(const Counted&) = default;
  Counted& operator=(Counted&&) = default;
  ~Counted() { --live; }
};
Counted cop(Counted a, Counted b) { return Counted(a.value + b.value); }
Counted ce() { return Counted(0); }
Counted cmap(int, Counted x) { return x; }
int ccomp(int, int) { return 0; }
int cid() { return 0; }
using CountTree = dynamic_lazy_segtree<Counted, cop, ce, int, cmap, ccomp, cid>;
void check_ownership() {
  static_assert(!std::is_copy_constructible_v<CountTree>);
  static_assert(!std::is_copy_assignable_v<CountTree>);
  static_assert(std::is_nothrow_move_constructible_v<CountTree>);
  assert(Counted::live == 0);
  {
    CountTree a(0, 8, [](ll l, ll r) { return Counted(r - l); });
    assert(a.max_right(0, [](Counted x) { return x.value <= 3; }) == 3);
    CountTree b(std::move(a));
    assert(a.root == nullptr);
    assert(b.prod(0, 8).value == 8);
    CountTree c(0, 2);
    c.apply(0, 1, 0);
    c = std::move(b);
    assert(b.root == nullptr);
    assert(c.prod(0, 8).value == 8);
    c = std::move(c);
    assert(c.prod(0, 8).value == 8);
  }
  assert(Counted::live == 0);
}
int main() {
  check_ownership();
  auto init = [](ll l, ll r) -> S { return {r - l, r - l}; };
  Tree first(0, 5, init), second(0, 5, init);
  auto le_two = [](S x) { return x.sum <= 2; };
  assert(first.max_right(0, le_two) == 2);
  assert(second.min_left(5, le_two) == 3);

  constexpr ll lo = -1000000000000LL, hi = 1000000000000LL;
  Tree wide(lo, hi, init);
  assert(wide.max_right(lo, le_two) == lo + 2);
  assert(wide.min_left(hi, le_two) == hi - 2);
  wide.apply(-2, 3, 4);
  assert(wide.prod(-3, 4).sum == 27);

  constexpr int n = 21;
  Tree tree(0, n, init);
  std::vector<long long> a(n, 1);
  std::mt19937 rng(54321);
  for (int step = 0; step < 1200; ++step) {
    int l = static_cast<int>(rng() % (n + 1));
    int r = static_cast<int>(rng() % (n + 1));
    if (l > r) std::swap(l, r);
    if (step % 3 == 0) {
      long long add = rng() % 4;
      tree.apply(l, r, add);
      for (int i = l; i < r; ++i) a[i] += add;
    }
    long long expected = 0;
    for (int i = l; i < r; ++i) expected += a[i];
    S actual = tree.prod(l, r);
    assert(actual.sum == expected && actual.len == r - l);
    long long limit = rng() % 120;
    auto check = [limit](S x) { return x.sum <= limit; };
    int right = l;
    long long sum = 0;
    while (right < n && sum + a[right] <= limit) sum += a[right++];
    assert(tree.max_right(l, check) == right);
    int left = r;
    sum = 0;
    while (left > 0 && sum + a[left - 1] <= limit) sum += a[--left];
    assert(tree.min_left(r, check) == left);
  }
}
