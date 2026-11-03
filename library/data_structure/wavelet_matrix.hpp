#pragma once

#include <algorithm>
#include <bit>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

namespace cp {

// Static integer sequence; coordinates are compressed in ascending order.
template <std::integral T> class WaveletMatrix {
 public:
  explicit WaveletMatrix(const std::vector<T>& values) : n_(values.size()), keys_(values) {
    std::sort(keys_.begin(), keys_.end());
    keys_.erase(std::unique(keys_.begin(), keys_.end()), keys_.end());
    levels_ = keys_.size() < 2 ? 0 : std::bit_width(keys_.size() - 1);
    std::vector<std::size_t> current;
    current.reserve(n_);
    for (const auto value : values) current.push_back(index(value));
    for (std::size_t level = 0; level < levels_; ++level) {
      const auto shift = levels_ - level - 1;
      Level bits;
      bits.words.assign((n_ / 64) + (n_ % 64 != 0), 0);
      std::vector<std::size_t> zero, one;
      zero.reserve(n_); one.reserve(n_);
      for (std::size_t i = 0; i < n_; ++i) {
        const bool bit = ((current[i] >> shift) & 1U) != 0;
        if (bit) bits.words[i / 64] |= std::uint64_t{1} << (i % 64);
        (bit ? one : zero).push_back(current[i]);
      }
      zeros_.push_back(zero.size());
      bits.prefix.resize(bits.words.size()+1, 0);
      for (std::size_t i=0;i<bits.words.size();++i)
        bits.prefix[i+1] = bits.prefix[i] + static_cast<std::size_t>(std::popcount(bits.words[i]));
      ones_.push_back(std::move(bits));
      zero.insert(zero.end(), one.begin(), one.end());
      current = std::move(zero);
    }
  }

  std::size_t size() const { return n_; }
  // k is zero-based; [l,r) must be nonempty and k < r-l.
  T kth(std::size_t l, std::size_t r, std::size_t k) const {
    valid(l,r); assert(k < r-l);
    std::size_t code = 0;
    for (std::size_t level = 0; level < levels_; ++level) {
      const auto l1 = ones_[level].rank(l), r1 = ones_[level].rank(r);
      const auto count0 = (r-l) - (r1-l1);
      code <<= 1;
      if (k < count0) { l -= l1; r -= r1; }
      else { k -= count0; code |= 1; l = zeros_[level]+l1; r = zeros_[level]+r1; }
    }
    return keys_[code];
  }
  std::size_t count_less(std::size_t l, std::size_t r, T x) const {
    valid(l,r);
    const auto code = index(x);
    if (code == keys_.size()) return r-l;
    std::size_t result = 0;
    for (std::size_t level = 0; level < levels_; ++level) {
      const auto l1 = ones_[level].rank(l), r1 = ones_[level].rank(r);
      const auto shift = levels_ - level - 1;
      if ((code >> shift) & 1U) {
        result += (r-l) - (r1-l1);
        l = zeros_[level]+l1; r = zeros_[level]+r1;
      } else { l -= l1; r -= r1; }
    }
    return result;
  }
  std::size_t count(std::size_t l, std::size_t r, T x) const {
    valid(l,r);
    const auto it = std::lower_bound(keys_.begin(), keys_.end(), x);
    if (it == keys_.end() || *it != x) return 0;
    const auto code = static_cast<std::size_t>(it - keys_.begin());
    for (std::size_t level = 0; level < levels_; ++level) {
      const auto l1 = ones_[level].rank(l), r1 = ones_[level].rank(r);
      const auto shift = levels_ - level - 1;
      if ((code >> shift) & 1U) { l = zeros_[level]+l1; r = zeros_[level]+r1; }
      else { l -= l1; r -= r1; }
    }
    return r-l;
  }
  std::size_t range_count(std::size_t l, std::size_t r, T lo, T hi) const {
    valid(l,r);
    return lo < hi ? count_less(l,r,hi) - count_less(l,r,lo) : 0;
  }
  std::optional<T> predecessor(std::size_t l, std::size_t r, T x) const {
    const auto k = count_less(l,r,x);
    return k == 0 ? std::nullopt : std::optional<T>(kth(l,r,k-1));
  }
  std::optional<T> lower_bound(std::size_t l, std::size_t r, T x) const {
    const auto k = count_less(l,r,x);
    return k == r-l ? std::nullopt : std::optional<T>(kth(l,r,k));
  }

 private:
  struct Level {
    std::vector<std::uint64_t> words;
    std::vector<std::size_t> prefix;
    std::size_t rank(std::size_t i) const {
      const auto block = i / 64, offset = i % 64;
      auto result = prefix[block];
      if (offset != 0) result += static_cast<std::size_t>(std::popcount(words[block] & ((std::uint64_t{1} << offset) - 1)));
      return result;
    }
  };
  std::size_t n_, levels_ = 0;
  std::vector<T> keys_;
  std::vector<std::size_t> zeros_;
  std::vector<Level> ones_;
  std::size_t index(T x) const { return static_cast<std::size_t>(std::lower_bound(keys_.begin(), keys_.end(), x)-keys_.begin()); }
  void valid([[maybe_unused]] std::size_t l, [[maybe_unused]] std::size_t r) const { assert(l <= r && r <= n_); }
};
}  // namespace cp
