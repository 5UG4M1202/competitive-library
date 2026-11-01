// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
// Intended correctness, including replacing a previous mapping on rebuild.
#include "../../library/data_structure/coord_compressor.hpp"
#include <algorithm>
#include <cassert>
#include <random>
#include <vector>

int main() {
  CoordCompressor<int> empty;
  std::vector<int> v;
  auto [nothing] = empty.build(v);
  assert(nothing.empty());
  std::mt19937 rng(81921);
  for (int trial = 0; trial < 100; ++trial) {
    std::vector<int> a(15), b(8), pool;
    for (auto& x : a) x = static_cast<int>(rng() % 21) - 10;
    for (auto& x : b) x = static_cast<int>(rng() % 21) - 10;
    pool = a;
    pool.insert(pool.end(), b.begin(), b.end());
    std::sort(pool.begin(), pool.end());
    pool.erase(std::unique(pool.begin(), pool.end()), pool.end());
    CoordCompressor<int> cc;
    auto [ca, cb] = cc.build(a, b);
    for (std::size_t i = 0; i < a.size(); ++i) {
      assert(ca[i] == std::lower_bound(pool.begin(), pool.end(), a[i]) - pool.begin());
      assert(cc.decompress(ca[i]) == a[i]);
    }
    assert(cc.decompress_vector(cb) == b);
    CoordCompressor<int> two;
    auto matrix = two.build2D({a, b});
    assert(matrix[0] == ca && matrix[1] == cb);
  }
  CoordCompressor<int> rebuilt;
  std::vector<int> first{10, 30, 40}, second{20};
  rebuilt.build(first);
  rebuilt.build(second);
  assert(rebuilt.compressed_map.count(10) == 0);
  assert(rebuilt.reverse_compressed_map.count(1) == 0);
  assert(rebuilt.compress(20) == 0 && rebuilt.decompress(0) == 20);
  rebuilt.build2D({});
  assert(rebuilt.compressed_map.empty() && rebuilt.reverse_compressed_map.empty());
  for (int trial = 0; trial < 100; ++trial) {
    std::vector<int> values(rng() % 15);
    for (int& x : values) x = static_cast<int>(rng() % 31) - 15;
    std::vector<int> sorted = values;
    std::sort(sorted.begin(), sorted.end());
    sorted.erase(std::unique(sorted.begin(), sorted.end()), sorted.end());
    if (trial % 2 == 0) rebuilt.build(values);
    else rebuilt.build2D({values});
    assert(rebuilt.compressed_map.size() == sorted.size());
    assert(rebuilt.reverse_compressed_map.size() == sorted.size());
    for (std::size_t i = 0; i < sorted.size(); ++i) {
      assert(rebuilt.compress(sorted[i]) == static_cast<int>(i));
      assert(rebuilt.decompress(static_cast<int>(i)) == sorted[i]);
    }
  }
}
