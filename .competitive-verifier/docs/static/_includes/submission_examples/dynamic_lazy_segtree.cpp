#include <iostream>

int main() {
  using P = cp::RangeAdd<long long, cp::AggregateKind::sum>;
  using Tree = dynamic_lazy_segtree<P::value_type, P::op, P::id,
      P::action_type, P::mapping, P::composition, P::action_id>;
  Tree seg(0, 1000000000LL,
           [](long long l, long long r) -> P::value_type { return {0, r - l}; });
  seg.apply(2, 5, 3);
  auto sum = seg.prod(0, 10).value;
  std::cout << sum << '\n'; // 9
}
