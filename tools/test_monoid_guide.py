#!/usr/bin/env python3
"""Compile the actual guide definition and exercise its documented uses."""
from pathlib import Path
import re
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
doc = (root / 'library/data_structure/monoid_segtree.md').read_text(encoding='utf-8')
match = re.search(r'```cpp\n([^`]*struct Minimum\s*\{[^`]*?)\n```', doc)
if not match:
    raise SystemExit('Minimum definition not found in Monoid guide')

main = r'''
#include "library/data_structure/monoid_segtree.hpp"
#include "library/data_structure/sparse_segtree_2d.hpp"
#include "library/data_structure/dynamic_segtree_2d.hpp"
#include <cassert>
#include <vector>
int main() {
  const auto id = Minimum::id();
  const std::vector<long long> values = {-3, 0, 7, id};
  for (auto a : values) {
    assert(Minimum::op(a,id)==a && Minimum::op(id,a)==a);
    for (auto b : values) {
      assert(Minimum::op(a,b)==Minimum::op(b,a));
      for (auto c : values)
        assert(Minimum::op(Minimum::op(a,b),c)==Minimum::op(a,Minimum::op(b,c)));
    }
  }
  cp::Segtree<Minimum> one(values);
  assert(one.prod(0,4)==-3 && one.prod(1,1)==id);
  cp::SparseSegtree2D<long long,Minimum> fixed({{1,2},{3,4}});
  cp::DynamicSegtree2D<Minimum> online(-10,10,-10,10);
  fixed.set(1,2,7); fixed.set(3,4,-3);
  online.set(1,2,7); online.set(3,4,-3);
  assert(fixed.prod(0,5,0,5)==-3 && online.prod(0,5,0,5)==-3);
  assert(fixed.prod(2,2,0,5)==id && online.prod(2,2,0,5)==id);
  using C = cp::MinCount<long long>;
  auto both=C::op(C::leaf(id),C::leaf(id));
  assert(both.value==id && both.count==2 && C::id().count==0);
  using G=cp::Gcd<unsigned long long>;
  assert(G::op(0,0)==0 && G::op(12,18)==6);
  using S=cp::RangeAggregate<long long,cp::AggregateKind::sum>;
  auto sum=S::op(S::leaf(3),S::leaf(4));
  assert(sum.value==7 && sum.length==2 && S::id().length==0);
}
'''
with tempfile.TemporaryDirectory(prefix='monoid-guide-') as temp:
    src = Path(temp) / 'guide.cpp'
    exe = Path(temp) / 'guide'
    src.write_text(match.group(1) + '\n' + main, encoding='utf-8')
    subprocess.run(['g++', '-std=c++20', '-O2', '-I', str(root), str(src), '-o', str(exe)], check=True)
    subprocess.run([str(exe)], check=True)
print('Monoid guide example: identity, associativity, commutativity and tree usage passed')
