#include <algorithm>
#include <iostream>
#include <vector>
int main() {
  std::vector<int> a{4, 2, 4, 7};
  auto keys = a;
  std::sort(keys.begin(), keys.end());
  keys.erase(std::unique(keys.begin(), keys.end()), keys.end());
  for (auto& x : a) x = static_cast<int>(std::lower_bound(keys.begin(), keys.end(), x) - keys.begin());
  Mo mo(static_cast<int>(a.size()));
  mo.add_query(0, 4);
  mo.add_query(1, 3);
  std::vector<int> frequency(keys.size(), 0), answer(2);
  int distinct = 0;
  auto add = [&](int i) { if (frequency[a[i]]++ == 0) ++distinct; };
  auto remove = [&](int i) { if (--frequency[a[i]] == 0) --distinct; };
  mo.run(add, remove, [&](int id) { answer[id] = distinct; });
  for (int result : answer) std::cout << result << '\n';
}
