// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/data_structure/wavelet_matrix.hpp"
#include <algorithm>
#include <cassert>
#include <climits>
#include <random>
#include <vector>
int main() {
  cp::WaveletMatrix<long long> empty({});
  assert(empty.size()==0 && empty.count_less(0,0,0)==0 && !empty.predecessor(0,0,0));
  cp::WaveletMatrix<long long> extremes({LLONG_MIN,LLONG_MAX,LLONG_MIN});
  assert(extremes.kth(0,3,0)==LLONG_MIN && extremes.kth(0,3,2)==LLONG_MAX);
  assert(extremes.count(0,3,LLONG_MAX)==1 && extremes.count_less(0,3,LLONG_MAX)==2);
  cp::WaveletMatrix<unsigned long long> unsigned_extremes({0,ULLONG_MAX,1,ULLONG_MAX});
  assert(unsigned_extremes.kth(0,4,3)==ULLONG_MAX);
  assert(unsigned_extremes.count(0,4,ULLONG_MAX)==2);
  assert(unsigned_extremes.predecessor(0,4,ULLONG_MAX)==1);
  cp::WaveletMatrix<bool> flags(std::vector<bool>{true,false,true});
  assert(flags.count(0,3,true)==2 && !flags.kth(0,3,0));
  std::mt19937 rng(9927);
  for (std::size_t n : {63U,64U,65U,127U,128U,129U}) {
    std::vector<int> a(n);
    for (auto& v:a) v=static_cast<int>(rng()%101)-50;
    cp::WaveletMatrix<int> wm(a);
    for (std::size_t l=0;l<=n;++l) for (std::size_t r=l;r<=n;++r) {
      assert(wm.count_less(l,r,0)==static_cast<std::size_t>(std::count_if(a.begin()+static_cast<std::ptrdiff_t>(l),a.begin()+static_cast<std::ptrdiff_t>(r),[](int v){return v<0;})));
      assert(wm.count(l,r,a[n/2])==static_cast<std::size_t>(std::count(a.begin()+static_cast<std::ptrdiff_t>(l),a.begin()+static_cast<std::ptrdiff_t>(r),a[n/2])));
      assert(wm.count(l,r,1000)==0);
    }
  }
  for (int trial=0;trial<100;++trial) {
    const std::size_t n=rng()%18;
    std::vector<int> values(n);
    for (auto& x:values) x=static_cast<int>(rng()%11)-5;
    cp::WaveletMatrix<int> tree(values);
    for (std::size_t l=0;l<=n;++l) for (std::size_t r=l;r<=n;++r) {
      std::vector<int> part(values.begin()+static_cast<std::ptrdiff_t>(l),values.begin()+static_cast<std::ptrdiff_t>(r));
      std::sort(part.begin(),part.end());
      for (std::size_t k=0;k<part.size();++k) assert(tree.kth(l,r,k)==part[k]);
      for (int x=-7;x<=7;++x) {
        auto it=std::lower_bound(part.begin(),part.end(),x);
        assert(tree.count_less(l,r,x)==static_cast<std::size_t>(it-part.begin()));
        assert(tree.count(l,r,x)==static_cast<std::size_t>(std::count(part.begin(),part.end(),x)));
        assert(tree.lower_bound(l,r,x)==(it==part.end()?std::optional<int>{}:std::optional<int>{*it}));
        assert(tree.predecessor(l,r,x)==(it==part.begin()?std::optional<int>{}:std::optional<int>{*(it-1)}));
        for (int y=-7;y<=7;++y)
          assert(tree.range_count(l,r,x,y)==static_cast<std::size_t>(std::count_if(part.begin(),part.end(),[&](int v){return x<=v && v<y;})));
      }
    }
  }
}
