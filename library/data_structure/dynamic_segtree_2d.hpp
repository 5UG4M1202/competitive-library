#pragma once

#include <cassert>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <numeric>
#include <vector>

namespace cp {

// Sparse implicit x tree and compressed binary y trees. M must be commutative.
// In y trees, `bit` is the highest branching bit; a leaf has bit == -1.
template <class M> class DynamicSegtree2D {
  static_assert(std::numeric_limits<long long>::digits == 63, "64-bit long long required");
 public:
  using value_type = typename M::value_type;
  using coordinate_type = long long;

  DynamicSegtree2D(coordinate_type xl, coordinate_type xr, coordinate_type yl, coordinate_type yr)
      : xl_(xl), xr_(xr), yl_(yl), yr_(yr) { assert(xl < xr && yl < yr); }

  void set(coordinate_type x, coordinate_type y, value_type value) {
    assert(xl_ <= x && x < xr_ && yl_ <= y && y < yr_);
    root_ = set_x(root_, xl_, xr_, x, y, value);
  }
  value_type get(coordinate_type x, coordinate_type y) const {
    assert(xl_ <= x && x < xr_ && yl_ <= y && y < yr_);
    auto i = root_;
    auto l = xl_, r = xr_;
    while (i != none && l + 1 != r) {
      const auto m = std::midpoint(l, r);
      if (x < m) { i = xnodes_[i].left; r = m; }
      else { i = xnodes_[i].right; l = m; }
    }
    return i == none ? M::id() : get_y(xnodes_[i].yroot, y);
  }
  value_type prod(coordinate_type xl, coordinate_type xr, coordinate_type yl, coordinate_type yr) const {
    if (xl >= xr || yl >= yr) return M::id();
    return prod_x(root_, xl_, xr_, xl, xr, yl, yr);
  }

 private:
  static constexpr std::size_t none = std::numeric_limits<std::size_t>::max();
  struct XNode { std::size_t left = none, right = none, yroot = none; };
  struct YNode {
    std::uint64_t key = 0;
    int bit = -1;
    std::size_t left = none, right = none;
    value_type value = M::id();
  };
  coordinate_type xl_, xr_, yl_, yr_;
  std::size_t root_ = none;
  std::vector<XNode> xnodes_;
  std::vector<YNode> ynodes_;

  value_type yvalue(std::size_t i) const { return i == none ? M::id() : ynodes_[i].value; }
  // Flipping the sign bit preserves signed order in an unsigned binary trie.
  static std::uint64_t key_of(coordinate_type y) { return static_cast<std::uint64_t>(y) ^ (std::uint64_t{1} << 63); }
  std::size_t leaf(std::uint64_t key, const value_type& value) {
    const auto i = ynodes_.size();
    ynodes_.push_back(YNode{key, -1, none, none, value});
    return i;
  }
  std::size_t insert_y(std::size_t i, std::uint64_t key, int differing_bit, const value_type& value) {
    if (ynodes_[i].bit < differing_bit) {
      const auto added = leaf(key, value);
      const bool right = ((key >> differing_bit) & 1U) != 0;
      const auto left = right ? i : added, r = right ? added : i;
      const auto parent = ynodes_.size();
      ynodes_.push_back(YNode{key, differing_bit, left, r, M::op(yvalue(left), yvalue(r))});
      return parent;
    }
    if (ynodes_[i].bit == -1) { ynodes_[i].value = value; return i; }
    const auto bit = ynodes_[i].bit;
    if (((key >> bit) & 1U) == 0) { const auto child = insert_y(ynodes_[i].left, key, differing_bit, value); ynodes_[i].left = child; }
    else { const auto child = insert_y(ynodes_[i].right, key, differing_bit, value); ynodes_[i].right = child; }
    ynodes_[i].value = M::op(yvalue(ynodes_[i].left), yvalue(ynodes_[i].right));
    return i;
  }
  std::size_t set_y(std::size_t i, coordinate_type y, const value_type& value) {
    const auto key = key_of(y);
    if (i == none) return leaf(key, value);
    auto found = i;
    while (ynodes_[found].bit >= 0) {
      const auto bit = ynodes_[found].bit;
      found = ((key >> bit) & 1U) == 0 ? ynodes_[found].left : ynodes_[found].right;
    }
    const auto difference = key ^ ynodes_[found].key;
    const int bit = difference == 0 ? -1 : 63 - static_cast<int>(std::countl_zero(difference));
    return insert_y(i, key, bit, value);
  }
  value_type get_y(std::size_t i, coordinate_type y) const {
    const auto key = key_of(y);
    while (i != none && ynodes_[i].bit >= 0) {
      const auto bit = ynodes_[i].bit;
      i = ((key >> bit) & 1U) == 0 ? ynodes_[i].left : ynodes_[i].right;
    }
    return i == none || ynodes_[i].key != key ? M::id() : ynodes_[i].value;
  }
  std::size_t set_x(std::size_t i, coordinate_type l, coordinate_type r, coordinate_type x, coordinate_type y, const value_type& value) {
    if (i == none) { i = xnodes_.size(); xnodes_.emplace_back(); }
    value_type combined = value;
    if (l + 1 != r) {
      const auto m = std::midpoint(l, r);
      if (x < m) { const auto child = set_x(xnodes_[i].left, l, m, x, y, value); xnodes_[i].left = child; }
      else { const auto child = set_x(xnodes_[i].right, m, r, x, y, value); xnodes_[i].right = child; }
      const auto left = xnodes_[i].left, right = xnodes_[i].right;
      const auto a = left == none ? M::id() : get_y(xnodes_[left].yroot, y);
      const auto b = right == none ? M::id() : get_y(xnodes_[right].yroot, y);
      combined = M::op(a, b);
    }
    const auto yroot = set_y(xnodes_[i].yroot, y, combined);
    xnodes_[i].yroot = yroot;
    return i;
  }
  value_type prod_y(std::size_t i, std::uint64_t ql, std::uint64_t qr) const {
    if (i == none) return M::id();
    const auto bit = ynodes_[i].bit;
    const std::uint64_t mask = bit < 0 ? 0 : bit == 63 ? ~std::uint64_t{0} : (std::uint64_t{1} << (bit + 1)) - 1;
    const auto lo = ynodes_[i].key & ~mask, hi = lo | mask;
    if (hi < ql || qr <= lo) return M::id();
    if (ql <= lo && hi < qr) return ynodes_[i].value;
    return M::op(prod_y(ynodes_[i].left, ql, qr), prod_y(ynodes_[i].right, ql, qr));
  }
  value_type prod_x(std::size_t i, coordinate_type l, coordinate_type r, coordinate_type ql, coordinate_type qr, coordinate_type yl, coordinate_type yr) const {
    if (i == none || r <= ql || qr <= l) return M::id();
    if (ql <= l && r <= qr) return prod_y(xnodes_[i].yroot, key_of(yl), key_of(yr));
    const auto m = std::midpoint(l, r);
    return M::op(prod_x(xnodes_[i].left, l, m, ql, qr, yl, yr), prod_x(xnodes_[i].right, m, r, ql, qr, yl, yr));
  }
};
}  // namespace cp
