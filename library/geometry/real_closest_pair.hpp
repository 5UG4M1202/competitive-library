#pragma once

#include "real_point.hpp"
#include <algorithm>
#include <cstddef>
#include <optional>
#include <vector>

namespace cp::geometry {

struct RealClosestPair {
  std::size_t first, second;  // Distinct indices into the input.
  long double distance2;
};

// Ideal real arithmetic: O(n log n) time and O(n) memory. Floating-point
// rounding can change answers and invalidate the geometric packing bound.
inline std::optional<RealClosestPair> closest_pair(const std::vector<RealPoint>& points) {
  if (points.size() < 2) return std::nullopt;
  struct Item { RealPoint p; std::size_t index; };
  std::vector<Item> work;
  work.reserve(points.size());
  for (std::size_t i = 0; i < points.size(); ++i) work.push_back({points[i], i});
  std::sort(work.begin(), work.end(), [](const Item& a, const Item& b) {
    if (a.p == b.p) return a.index < b.index;
    return a.p < b.p;
  });
  // Besides answering the zero-distance case, this ensures the strip packing
  // argument never degenerates to an unbounded number of coincident points.
  for (std::size_t i = 1; i < work.size(); ++i) {
    if (work[i - 1].p == work[i].p)
      return RealClosestPair{work[i - 1].index, work[i].index, 0};
  }

  std::vector<Item> scratch(work.size());
  std::vector<Item> strip;
  strip.reserve(work.size());
  const auto by_y = [](const Item& a, const Item& b) {
    if (a.p.y != b.p.y) return a.p.y < b.p.y;
    if (a.p.x != b.p.x) return a.p.x < b.p.x;
    return a.index < b.index;
  };
  const auto search = [&](auto&& self, std::size_t lo, std::size_t hi)
      -> std::optional<RealClosestPair> {
    if (hi - lo <= 1) return std::nullopt;
    const std::size_t mid = lo + (hi - lo) / 2;
    const long double split_x = work[mid].p.x;
    auto left = self(self, lo, mid);
    auto right = self(self, mid, hi);
    auto best = left;
    if (right && (!best || right->distance2 < best->distance2)) best = right;
    std::merge(work.begin() + lo, work.begin() + mid,
               work.begin() + mid, work.begin() + hi, scratch.begin() + lo, by_y);
    std::copy(scratch.begin() + lo, scratch.begin() + hi, work.begin() + lo);
    strip.clear();
    for (std::size_t i = lo; i < hi; ++i) {
      const long double dx = static_cast<long double>(work[i].p.x) - split_x;
      if (best && dx * dx > best->distance2) continue;
      for (auto it = strip.rbegin(); it != strip.rend(); ++it) {
        const long double dy = static_cast<long double>(work[i].p.y) - it->p.y;
        if (best && dy * dy > best->distance2) break;
        const long double d = squared_distance(work[i].p, it->p);
        if (!best || d < best->distance2)
          best = RealClosestPair{work[i].index, it->index, d};
      }
      strip.push_back(work[i]);
    }
    return best;
  };
  return search(search, 0, work.size());
}

}  // namespace cp::geometry
