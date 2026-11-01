// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/data_structure/dynamic_lazy_segtree.hpp"
#include "../../library/data_structure/monoid_segtree.hpp"
#include <cassert>

template <class P>
using Tree = dynamic_lazy_segtree<typename P::value_type, P::op, P::id,
    typename P::action_type, P::mapping, P::composition, P::action_id>;

int main() {
  using Add = cp::RangeAdd<long long, cp::AggregateKind::sum>;
  Tree<Add> add(0, 1000000000LL,
      [](long long l, long long r) -> Add::value_type { return {0, r - l}; });
  add.apply(2, 5, 3);
  assert(add.prod(0, 10).value == 9);

  using Assign = cp::RangeAssign<long long, cp::AggregateKind::sum>;
  Tree<Assign> assign(0, 10,
      [](long long l, long long r) -> Assign::value_type { return {0, r - l}; });
  assign.apply(0, 5, 4);
  assign.apply(2, 4, 1);
  assert(assign.prod(0, 10).value == 14);

  using Affine = cp::RangeAffineSum<long long>;
  Tree<Affine> affine(0, 10,
      [](long long l, long long r) -> Affine::value_type { return {0, r - l}; });
  affine.apply(0, 5, {1, 2});
  affine.apply(1, 4, {3, 1});
  assert(affine.prod(0, 5).value == 25);
}
