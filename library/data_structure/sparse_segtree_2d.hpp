#pragma once

#include <atcoder/segtree>
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <utility>
#include <vector>

namespace cp {

// All update coordinates must be supplied at construction. M must be commutative.
template <class Coord, class M> class SparseSegtree2D {
 public:
  using value_type = typename M::value_type;
  using point_type = std::pair<Coord, Coord>;

  explicit SparseSegtree2D(std::vector<point_type> points) {
    std::sort(points.begin(), points.end());
    points.erase(std::unique(points.begin(), points.end()), points.end());
    for (const auto& p : points) xs_.push_back(p.first);
    std::sort(xs_.begin(), xs_.end());
    xs_.erase(std::unique(xs_.begin(), xs_.end()), xs_.end());
    while (base_ < xs_.size()) base_ *= 2;
    ys_.resize(base_ * 2);
    for (const auto& p : points) {
      auto i = static_cast<std::size_t>(std::lower_bound(xs_.begin(), xs_.end(), p.first) - xs_.begin()) + base_;
      while (i != 0) { ys_[i].push_back(p.second); i /= 2; }
    }
    trees_.resize(base_ * 2);
    for (std::size_t i = 1; i < ys_.size(); ++i) {
      auto& ys = ys_[i];
      std::sort(ys.begin(), ys.end());
      ys.erase(std::unique(ys.begin(), ys.end()), ys.end());
      assert(ys.size() <= static_cast<std::size_t>(100000000)); // ACL precondition
      trees_[i] = tree_type(static_cast<int>(ys.size()));
    }
  }

  value_type get(const Coord& x, const Coord& y) const {
    const auto it = std::lower_bound(xs_.begin(), xs_.end(), x);
    if (it == xs_.end() || *it != x) return M::id();
    return at(static_cast<std::size_t>(it - xs_.begin()) + base_, y);
  }

  void set(const Coord& x, const Coord& y, value_type value) {
    const auto it = std::lower_bound(xs_.begin(), xs_.end(), x);
    assert(it != xs_.end() && *it == x);
    std::size_t i = static_cast<std::size_t>(it - xs_.begin()) + base_;
    auto yi = std::lower_bound(ys_[i].begin(), ys_[i].end(), y);
    assert(yi != ys_[i].end() && *yi == y);
    trees_[i].set(static_cast<int>(yi - ys_[i].begin()), value);
    while ((i /= 2) != 0) {
      yi = std::lower_bound(ys_[i].begin(), ys_[i].end(), y);
      trees_[i].set(static_cast<int>(yi - ys_[i].begin()), M::op(at(i * 2, y), at(i * 2 + 1, y)));
    }
  }

  value_type prod(const Coord& xl, const Coord& xr, const Coord& yl, const Coord& yr) const {
    if (!(xl < xr) || !(yl < yr)) return M::id();
    auto l = static_cast<std::size_t>(std::lower_bound(xs_.begin(), xs_.end(), xl) - xs_.begin()) + base_;
    auto r = static_cast<std::size_t>(std::lower_bound(xs_.begin(), xs_.end(), xr) - xs_.begin()) + base_;
    value_type result = M::id();
    while (l < r) {
      if (l & 1U) result = M::op(result, range(l++, yl, yr));
      if (r & 1U) result = M::op(result, range(--r, yl, yr));
      l /= 2; r /= 2;
    }
    return result;
  }

 private:
  using tree_type = atcoder::segtree<value_type, M::op, M::id>;
  std::size_t base_ = 1;
  std::vector<Coord> xs_;
  std::vector<std::vector<Coord>> ys_;
  std::vector<tree_type> trees_;

  value_type at(std::size_t i, const Coord& y) const {
    const auto it = std::lower_bound(ys_[i].begin(), ys_[i].end(), y);
    return it == ys_[i].end() || *it != y ? M::id() : trees_[i].get(static_cast<int>(it - ys_[i].begin()));
  }
  value_type range(std::size_t i, const Coord& lo, const Coord& hi) const {
    const auto l = std::lower_bound(ys_[i].begin(), ys_[i].end(), lo) - ys_[i].begin();
    const auto r = std::lower_bound(ys_[i].begin(), ys_[i].end(), hi) - ys_[i].begin();
    return trees_[i].prod(static_cast<int>(l), static_cast<int>(r));
  }
};
}  // namespace cp
