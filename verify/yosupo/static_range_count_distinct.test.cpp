// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/static_range_count_distinct
// competitive-verifier: DISPLAY no-index

#include "library/misc/mo.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int n, q;
  std::cin >> n >> q;
  std::vector<int> a(n);
  for (auto& x : a) std::cin >> x;
  auto keys = a;
  std::sort(keys.begin(), keys.end());
  keys.erase(std::unique(keys.begin(), keys.end()), keys.end());
  for (auto& x : a) {
    x = static_cast<int>(std::lower_bound(keys.begin(), keys.end(), x) - keys.begin());
  }
  Mo mo(n);
  for (int i = 0; i < q; ++i) {
    int l, r;
    std::cin >> l >> r;
    mo.add_query(l, r);
  }
  std::vector<int> frequency(keys.size()), answers(q);
  int distinct = 0;
  auto add = [&](int i) { if (frequency[a[i]]++ == 0) ++distinct; };
  auto remove = [&](int i) { if (--frequency[a[i]] == 0) --distinct; };
  mo.run(add, remove, [&](int id) { answers[id] = distinct; });
  for (int answer : answers) std::cout << answer << '\n';
}
