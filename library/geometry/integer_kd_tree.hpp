#pragma once

#include "integer_point.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <limits>
#include <numeric>
#include <vector>

namespace cp::geometry {

// Fixed point coordinates, point assignment, and commutative monoid rectangles.
template <class M>
class IntegerKdTree {
 public:
  using value_type = typename M::value_type;

  IntegerKdTree(const std::vector<IntegerPoint>& points,
                const std::vector<value_type>& values)
      : points_(points), locations_(points.size()) {
    assert(points.size() == values.size());
    assert(points.size() <= static_cast<std::size_t>(std::numeric_limits<int>::max()));
    std::vector<std::size_t> order(points.size());
    std::iota(order.begin(), order.end(), std::size_t{0});
    nodes_.reserve(points.size());
    root_ = build(order, values, 0, order.size(), 0, -1);
    std::vector<IntegerPoint>().swap(points_);  // Only needed during construction.
  }

  void set(std::size_t id, const value_type& value) {
    assert(id < locations_.size());
    int v = locations_[id];
    nodes_[v].value = value;
    while (v != -1) {
      pull(v);
      v = nodes_[v].parent;
    }
  }

  // Half-open boundaries are widened to allow INT64_MAX + 1.
  value_type prod(IntegerWide xl, IntegerWide xr,
                  IntegerWide yl, IntegerWide yr) const {
    if (xl >= xr || yl >= yr) return M::id();
    constexpr auto lo = std::numeric_limits<std::int64_t>::min();
    constexpr auto hi = std::numeric_limits<std::int64_t>::max();
    if (xr <= IntegerWide(lo) || IntegerWide(hi) < xl ||
        yr <= IntegerWide(lo) || IntegerWide(hi) < yl) return M::id();
    const auto left = xl <= IntegerWide(lo) ? lo : static_cast<std::int64_t>(xl);
    const auto right = xr > IntegerWide(hi) ? hi : static_cast<std::int64_t>(xr - 1);
    const auto bottom = yl <= IntegerWide(lo) ? lo : static_cast<std::int64_t>(yl);
    const auto top = yr > IntegerWide(hi) ? hi : static_cast<std::int64_t>(yr - 1);
    return query(root_, left, right, bottom, top);
  }

 private:
  struct Node {
    IntegerPoint point;
    int parent, left = -1, right = -1;
    std::int64_t xmin, xmax, ymin, ymax;
    value_type value, aggregate;
  };
  std::vector<IntegerPoint> points_;
  std::vector<int> locations_;
  std::vector<Node> nodes_;
  int root_ = -1;

  int build(std::vector<std::size_t>& order, const std::vector<value_type>& values,
            std::size_t l, std::size_t r, int axis, int parent) {
    if (l == r) return -1;
    const auto mid = l + (r - l) / 2;
    auto less = [&](std::size_t a, std::size_t b) {
      const auto p = points_[a], q = points_[b];
      const auto pa = axis == 0 ? p.x : p.y, qa = axis == 0 ? q.x : q.y;
      if (pa != qa) return pa < qa;
      const auto pb = axis == 0 ? p.y : p.x, qb = axis == 0 ? q.y : q.x;
      return pb != qb ? pb < qb : a < b;
    };
    std::nth_element(order.begin() + l, order.begin() + mid, order.begin() + r, less);
    const auto id = order[mid];
    const auto p = points_[id];
    const int v = static_cast<int>(nodes_.size());
    nodes_.push_back({p, parent, -1, -1, p.x, p.x, p.y, p.y, values[id], values[id]});
    locations_[id] = v;
    const int left = build(order, values, l, mid, 1 - axis, v);
    const int right = build(order, values, mid + 1, r, 1 - axis, v);
    nodes_[v].left = left;
    nodes_[v].right = right;
    for (int c : {left, right}) if (c != -1) {
      nodes_[v].xmin = std::min(nodes_[v].xmin, nodes_[c].xmin);
      nodes_[v].xmax = std::max(nodes_[v].xmax, nodes_[c].xmax);
      nodes_[v].ymin = std::min(nodes_[v].ymin, nodes_[c].ymin);
      nodes_[v].ymax = std::max(nodes_[v].ymax, nodes_[c].ymax);
    }
    pull(v);
    return v;
  }

  void pull(int v) {
    auto result = nodes_[v].value;
    if (nodes_[v].left != -1) result = M::op(result, nodes_[nodes_[v].left].aggregate);
    if (nodes_[v].right != -1) result = M::op(result, nodes_[nodes_[v].right].aggregate);
    nodes_[v].aggregate = result;
  }

  value_type query(int v, std::int64_t xl, std::int64_t xr,
                   std::int64_t yl, std::int64_t yr) const {
    if (v == -1) return M::id();
    const auto& n = nodes_[v];
    if (n.xmax < xl || xr < n.xmin || n.ymax < yl || yr < n.ymin) return M::id();
    if (xl <= n.xmin && n.xmax <= xr && yl <= n.ymin && n.ymax <= yr) return n.aggregate;
    const auto p = n.point;
    auto result = (xl <= p.x && p.x <= xr && yl <= p.y && p.y <= yr) ? n.value : M::id();
    result = M::op(result, query(n.left, xl, xr, yl, yr));
    return M::op(result, query(n.right, xl, xr, yl, yr));
  }
};

}  // namespace cp::geometry
