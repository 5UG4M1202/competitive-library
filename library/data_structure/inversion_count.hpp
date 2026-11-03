#pragma once
#include <algorithm>
#include <atcoder/fenwicktree>
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <vector>

namespace cp {
namespace detail {
inline void check_inversion_size(std::size_t n) {
  // ACL documents at most 10^8 entries for fenwick_tree.
  if (n > 100000000u) throw std::length_error("too many values");
}
template <bool FromRight, class T, class Compare, class Emit>
void for_each_inversion_contribution(const std::vector<T>& values, Compare comp, Emit emit) {
  check_inversion_size(values.size());
  auto keys = values;
  std::sort(keys.begin(), keys.end(), comp);
  keys.erase(std::unique(keys.begin(), keys.end(), [&](const T& a, const T& b) {
    return !comp(a, b) && !comp(b, a);
  }), keys.end());
  atcoder::fenwick_tree<long long> counts(static_cast<int>(keys.size()));
  for (std::size_t step = 0; step < values.size(); ++step) {
    const std::size_t i = FromRight ? values.size() - 1 - step : step;
    const int rank = static_cast<int>(std::lower_bound(keys.begin(), keys.end(), values[i], comp) - keys.begin());
    long long contribution;
    if constexpr (FromRight) contribution = counts.sum(0, rank);
    else contribution = static_cast<long long>(step) - counts.sum(0, rank + 1);
    emit(i, contribution);
    counts.add(rank, 1);
  }
}
}  // namespace detail

// Count pairs i < j for which values[j] precedes values[i] under comp.
template <class T, class Compare = std::less<>>
long long inversion_count(const std::vector<T>& values, Compare comp = {}) {
  long long result = 0;
  detail::for_each_inversion_contribution<false>(values, comp,
      [&](std::size_t, long long contribution) { result += contribution; });
  return result;
}

// For each j, count i < j with values[j] preceding values[i].
template <class T, class Compare = std::less<>>
std::vector<long long> inversion_count_ending_at(const std::vector<T>& values, Compare comp = {}) {
  detail::check_inversion_size(values.size());
  std::vector<long long> result(values.size());
  detail::for_each_inversion_contribution<false>(values, comp,
      [&](std::size_t i, long long contribution) { result[i] = contribution; });
  return result;
}

// For each i, count j > i with values[j] preceding values[i].
template <class T, class Compare = std::less<>>
std::vector<long long> inversion_count_starting_at(const std::vector<T>& values, Compare comp = {}) {
  detail::check_inversion_size(values.size());
  std::vector<long long> result(values.size());
  detail::for_each_inversion_contribution<true>(values, comp,
      [&](std::size_t i, long long contribution) { result[i] = contribution; });
  return result;
}
}  // namespace cp
