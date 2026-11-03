// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/data_structure/sparse_segtree_2d.hpp"
#include "../../library/data_structure/dynamic_segtree_2d.hpp"
#include "../../library/math/rational128.hpp"
#include <algorithm>
#include <bit>
#include <cassert>
#include <climits>
#include <map>
#include <numeric>
#include <random>
#include <vector>

struct Sum { using value_type = long long; static long long id() { return 0; } static long long op(long long a, long long b) { return a+b; } };
struct Gcd { using value_type = long long; static long long id() { return 0; } static long long op(long long a, long long b) { return std::gcd(a,b); } };
struct FractionSum { using value_type = cp::Rational128; static value_type id() { return 0; } static value_type op(value_type a, value_type b) { return a+b; } };

template <class M> void randomized() {
  std::mt19937 rng(4351);
  std::vector<std::pair<long long,long long>> points;
  for (int i=0;i<100;++i) points.emplace_back(static_cast<long long>(rng()%21)-10, static_cast<long long>(rng()%21)-10);
  points.push_back(points[0]); // duplicate registrations refer to the same cell
  cp::SparseSegtree2D<long long,M> fixed(points);
  cp::DynamicSegtree2D<M> online(-11,11,-11,11);
  std::map<std::pair<long long,long long>,long long> values;
  for (int step=0;step<2500;++step) {
    if (rng()%2 == 0) {
      const auto p = points[rng()%points.size()];
      const long long v = static_cast<long long>(rng()%30);
      fixed.set(p.first,p.second,v); online.set(p.first,p.second,v); values[p]=v;
      assert(fixed.get(p.first,p.second)==v && online.get(p.first,p.second)==v);
    } else {
      auto xl=static_cast<long long>(rng()%27)-13, xr=static_cast<long long>(rng()%27)-13;
      auto yl=static_cast<long long>(rng()%27)-13, yr=static_cast<long long>(rng()%27)-13;
      if (xl>xr) std::swap(xl,xr);
      if (yl>yr) std::swap(yl,yr);
      long long expected=M::id();
      for (const auto& [p,v] : values) if (xl<=p.first && p.first<xr && yl<=p.second && p.second<yr) expected=M::op(expected,v);
      assert(fixed.prod(xl,xr,yl,yr)==expected);
      assert(online.prod(xl,xr,yl,yr)==expected);
    }
  }
  assert(fixed.prod(1,1,-20,20)==M::id() && online.prod(1,1,-20,20)==M::id());
}
int main() {
  randomized<Sum>(); randomized<Gcd>();
  cp::SparseSegtree2D<long long,Sum> empty({});
  assert(empty.get(1,2)==0 && empty.prod(-1,3,-2,4)==0);
  cp::DynamicSegtree2D<Sum> extremes(LLONG_MIN,LLONG_MAX,LLONG_MIN,LLONG_MAX);
  extremes.set(LLONG_MIN,LLONG_MAX-1,17);
  extremes.set(LLONG_MAX-1,LLONG_MIN,23);
  assert(extremes.get(LLONG_MIN,LLONG_MAX-1)==17);
  assert(extremes.prod(LLONG_MIN,LLONG_MAX,LLONG_MIN,LLONG_MAX)==40);
  assert(extremes.prod(0,LLONG_MAX,LLONG_MIN,0)==23);
  std::mt19937_64 wide_rng(12345);
  std::map<std::pair<long long,long long>,long long> wide_values;
  auto wide = [&] { const auto x = std::bit_cast<long long>(wide_rng()); return x == LLONG_MAX ? x-1 : x; };
  for (int i=0;i<700;++i) {
    const auto x=wide(), y=wide();
    extremes.set(x,y,1); wide_values[{x,y}]=1;
  }
  wide_values[{LLONG_MIN,LLONG_MAX-1}]=17;
  wide_values[{LLONG_MAX-1,LLONG_MIN}]=23;
  for (int i=0;i<1000;++i) {
    auto xl=wide(), xr=wide(), yl=wide(), yr=wide();
    if (xl>xr) std::swap(xl,xr);
    if (yl>yr) std::swap(yl,yr);
    long long expected=0;
    for (const auto& [p,v] : wide_values) if (xl<=p.first && p.first<xr && yl<=p.second && p.second<yr) expected+=v;
    assert(extremes.prod(xl,xr,yl,yr)==expected);
  }
  using R = cp::Rational128;
  cp::SparseSegtree2D<R,FractionSum> fractions({{R(1,2),R(2,3)}, {R(1,3),R(1,4)}});
  fractions.set(R(1,2),R(2,3),R(1,5)); fractions.set(R(1,3),R(1,4),R(2,5));
  assert(fractions.prod(R(0),R(1),R(0),R(1)) == R(3,5));
  cp::DynamicSegtree2D<FractionSum> online_fractions(-10,10,-10,10);
  online_fractions.set(1,2,R(1,3)); online_fractions.set(2,3,R(1,6));
  assert(online_fractions.prod(0,5,0,5)==R(1,2));
}
