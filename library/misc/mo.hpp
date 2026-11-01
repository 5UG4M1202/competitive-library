#pragma once
#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
#include <numeric>
#include <utility>
#include <vector>

// Based on the legacy @mos workflow; its historical provenance is unconfirmed.
class Mo {
  int n;
  std::vector<std::pair<int, int>> queries;

 public:
  explicit Mo(int length) : n(length) { assert(length >= 0); }

  int add_query(int l, int r) {
    assert(0 <= l && l <= r && r <= n);
    assert(queries.size() < static_cast<std::size_t>(std::numeric_limits<int>::max()));
    int id = static_cast<int>(queries.size());
    queries.emplace_back(l, r);
    return id;
  }

  template <class AddLeft, class AddRight, class RemoveLeft, class RemoveRight, class Answer>
  void run(AddLeft add_left, AddRight add_right, RemoveLeft remove_left,
           RemoveRight remove_right, Answer answer) const {
    int q = static_cast<int>(queries.size());
    if (q == 0) return;
    int width = std::max(1, static_cast<int>(n / std::sqrt(static_cast<double>(q))));
    std::vector<int> order(q);
    std::iota(order.begin(), order.end(), 0);
    std::sort(order.begin(), order.end(), [&](int a, int b) {
      int ba = queries[a].first / width, bb = queries[b].first / width;
      if (ba != bb) return ba < bb;
      return ba % 2 ? queries[a].second > queries[b].second : queries[a].second < queries[b].second;
    });
    int l = 0, r = 0;
    for (int id : order) {
      int ql = queries[id].first, qr = queries[id].second;
      while (l > ql) add_left(--l);
      while (r < qr) add_right(r++);
      while (l < ql) remove_left(l++);
      while (r > qr) remove_right(--r);
      answer(id);
    }
  }
};
