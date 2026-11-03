#pragma once
#include <algorithm>
#include <cassert>
#include <cmath>
#include <functional>
#include <limits>
#include <numeric>
#include <utility>
#include <vector>

// Based on the legacy @mos workflow; its historical provenance is unconfirmed.
class Mo {
  int n;
  int block_size = 0;
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

  // Zero restores the automatic width; a positive value is used as-is.
  void set_block_size(int size) { assert(size >= 0); block_size = size; }

  // Use the same add/remove callbacks for both endpoints.
  template <class Add, class Remove, class Answer>
  void run(Add&& add, Remove&& remove, Answer&& answer) const {
    run(std::ref(add), std::ref(add), std::ref(remove), std::ref(remove), std::ref(answer));
  }

  template <class AddLeft, class AddRight, class RemoveLeft, class RemoveRight, class Answer>
  void run(AddLeft add_left, AddRight add_right, RemoveLeft remove_left,
           RemoveRight remove_right, Answer answer) const {
    int q = static_cast<int>(queries.size());
    if (q == 0) return;
    int width = block_size ? block_size : std::max(1, static_cast<int>(n / std::sqrt(static_cast<double>(q))));
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
