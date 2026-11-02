// Reproducible synthetic performance probe; not a correctness test.
#ifdef BENCH_REAL_KD_TREE
#include "../library/geometry/real_kd_tree.hpp"
#else
#include "../library/geometry/integer_kd_tree.hpp"
#endif
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include <vector>

struct Sum {
  using value_type = long long;
  static long long id() { return 0; }
  static long long op(long long a, long long b) { return a + b; }
};

#ifdef BENCH_REAL_KD_TREE
using BenchPoint = cp::geometry::RealPoint;
using BenchTree = cp::geometry::RealKdTree<Sum>;
#else
using BenchPoint = cp::geometry::IntegerPoint;
using BenchTree = cp::geometry::IntegerKdTree<Sum>;
#endif
using BenchCoord = decltype(BenchPoint{}.x);

int main(int argc, char** argv) {
  const std::string mode = argc > 1 ? argv[1] : "random";
  const int n = argc > 2 ? std::atoi(argv[2]) : 200000;
  const int q = argc > 3 ? std::atoi(argv[3]) : 200000;
  std::mt19937 rng(32131);
  std::vector<BenchPoint> p;
  std::vector<long long> a(n, 1);
  for (int i = 0; i < n; ++i) {
    if (mode == "grid") p.push_back({static_cast<BenchCoord>(i % 500), static_cast<BenchCoord>(i / 500)});
    else if (mode == "line") p.push_back({static_cast<BenchCoord>(i), 0});
    else if (mode == "duplicates") p.push_back({static_cast<BenchCoord>(i % 10), static_cast<BenchCoord>(i % 10)});
    else p.push_back({static_cast<BenchCoord>(rng() % 1000000), static_cast<BenchCoord>(rng() % 1000000)});
  }
  const auto start = std::chrono::steady_clock::now();
  BenchTree tree(p, a);
  const auto built = std::chrono::steady_clock::now();
  long long checksum = 0;
  for (int i = 0; i < q; ++i) {
    if (mode == "grid") {
      const int x = static_cast<int>(rng() % 500), y = static_cast<int>(rng() % 400);
      checksum += tree.prod(x, x + 1, y, 400);
    } else if (mode == "line") {
      const int x = static_cast<int>(rng() % static_cast<unsigned>(n));
      checksum += tree.prod(x, x + 100, -1, 1);
    } else if (mode == "duplicates") {
      const int x = static_cast<int>(rng() % 10);
      checksum += tree.prod(x, x + 1, x, x + 1);
    } else {
      int x = static_cast<int>(rng() % 1000000), y = static_cast<int>(rng() % 1000000);
      checksum += tree.prod(x, x + 1000, y, y + 1000);
    }
  }
  const auto end = std::chrono::steady_clock::now();
  std::cout << mode << " n=" << n << " q=" << q
            << " build_ms=" << std::chrono::duration_cast<std::chrono::milliseconds>(built - start).count()
            << " query_ms=" << std::chrono::duration_cast<std::chrono::milliseconds>(end - built).count()
            << " checksum=" << checksum << '\n';
}
