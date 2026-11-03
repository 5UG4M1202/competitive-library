// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/misc/mo.hpp"
#include <cassert>
#include <deque>
#include <memory>
#include <random>
#include <vector>

int main() {
  Mo empty(0);
  bool called = false;
  auto ignore = [](int) {};
  empty.run(ignore, ignore, ignore, ignore, [&](int) { called = true; });
  assert(!called);
  std::mt19937 rng(424242);
  for (int n = 0; n <= 60; ++n) {
    for (int trial = 0; trial < 20; ++trial) {
      Mo mo(n);
      if (trial % 3 == 1) mo.set_block_size(1);
      if (trial % 3 == 2) mo.set_block_size(n+1);
      std::vector<std::pair<int, int>> queries;
      for (int i = 0; i < 100; ++i) {
        int l = static_cast<int>(rng() % (n + 1));
        int r = static_cast<int>(rng() % (n + 1));
        if (l > r) std::swap(l, r);
        assert(mo.add_query(l, r) == i);
        queries.emplace_back(l, r);
      }
      std::deque<int> active;
      std::vector<int> visits(queries.size());
      mo.run([&](int i) { active.push_front(i); },
             [&](int i) { active.push_back(i); },
             [&](int i) { assert(!active.empty() && active.front() == i); active.pop_front(); },
             [&](int i) { assert(!active.empty() && active.back() == i); active.pop_back(); },
             [&](int id) {
               ++visits[id];
               auto [l, r] = queries[id];
               assert(static_cast<int>(active.size()) == r - l);
               for (int i = l; i < r; ++i) assert(active[i - l] == i);
             });
      for (int count : visits) assert(count == 1);
    }
  }

  // Symmetric callbacks can own noncopyable state, and a second run starts empty.
  Mo symmetric(5);
  symmetric.set_block_size(4);
  symmetric.set_block_size(0);
  symmetric.add_query(0,3); symmetric.add_query(1,5); symmetric.add_query(2,2);
  int sum=0;
  auto add_move_only = [token=std::make_unique<int>(1), &sum](int i) { sum += *token * i; };
  auto remove_move_only = [token=std::make_unique<int>(1), &sum](int i) { sum -= *token * i; };
  std::vector<int> sums(3);
  for (int pass=0;pass<2;++pass) {
    sum=0;
    symmetric.run(add_move_only,remove_move_only,[&](int id){sums[id]=sum;});
    assert((sums==std::vector<int>{3,10,0}));
  }

  // Inversions require different left/right operations; compare with brute force.
  for (int trial=0;trial<100;++trial) {
    constexpr int n=30;
    std::vector<int> a(n);
    for (auto& x:a) x=static_cast<int>(rng()%8);
    Mo mo(n);
    std::vector<std::pair<int,int>> ranges;
    for (int i=0;i<60;++i) {
      int l=static_cast<int>(rng()%(n+1)), r=static_cast<int>(rng()%(n+1));
      if(l>r) std::swap(l,r);
      mo.add_query(l,r); ranges.emplace_back(l,r);
    }
    std::vector<int> freq(8);
    long long inversions=0;
    auto less = [&](int x) { int count=0; for(int v=0;v<x;++v) count+=freq[v]; return count; };
    auto greater = [&](int x) { int count=0; for(int v=x+1;v<8;++v) count+=freq[v]; return count; };
    mo.run([&](int i){inversions+=less(a[i]); ++freq[a[i]];},
           [&](int i){inversions+=greater(a[i]); ++freq[a[i]];},
           [&](int i){--freq[a[i]]; inversions-=less(a[i]);},
           [&](int i){--freq[a[i]]; inversions-=greater(a[i]);},
           [&](int id){
             auto [l,r]=ranges[id]; long long expected=0;
             for(int i=l;i<r;++i) for(int j=i+1;j<r;++j) expected+=a[i]>a[j];
             assert(inversions==expected);
           });
  }
}
