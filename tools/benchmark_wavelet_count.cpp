#include "../library/data_structure/wavelet_matrix.hpp"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>
int main() {
  constexpr std::size_t n = 200000, q = 1000000;
  std::mt19937 rng(20261104);
  std::vector<int> values(n);
  for (auto& value : values) value = static_cast<int>(rng() % 50000);
  std::vector<std::size_t> left(q), right(q);
  std::vector<int> query(q);
  for (std::size_t i = 0; i < q; ++i) {
    auto a = rng() % n, b = rng() % n;
    if (a > b) std::swap(a, b);
    left[i] = a;
    right[i] = b + 1;
    query[i] = values[rng() % n];
  }
  cp::WaveletMatrix<int> wm(values);
  const auto start = std::chrono::steady_clock::now();
  std::uint64_t checksum = 0;
  for (std::size_t i = 0; i < q; ++i) checksum += wm.count(left[i], right[i], query[i]);
  const auto end = std::chrono::steady_clock::now();
  std::cout << "checksum=" << checksum << " count_ms="
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << '\n';
}
