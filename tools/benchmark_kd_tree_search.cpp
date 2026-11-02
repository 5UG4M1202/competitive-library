// Fixed-seed performance probe; timings are not correctness guarantees.
#ifdef BENCH_REAL_KD_TREE
#include "../library/geometry/real_kd_tree.hpp"
#else
#include "../library/geometry/integer_kd_tree.hpp"
#endif
#include <chrono>
#include <iostream>
#include <random>
#include <vector>
struct Sum {
  using value_type = int;
  static int id() { return 0; }
  static int op(int a,int b) { return a+b; }
};
#ifdef BENCH_REAL_KD_TREE
using Point = cp::geometry::RealPoint;
using Tree = cp::geometry::RealKdTree<Sum>;
#else
using Point = cp::geometry::IntegerPoint;
using Tree = cp::geometry::IntegerKdTree<Sum>;
#endif
using Coord = decltype(Point{}.x);
int main() {
  constexpr int n=200000, queries=20000;
  std::mt19937 rng(9912);
  std::vector<Point> points;
  points.reserve(n);
  for (int i=0;i<n;++i)
    points.push_back({static_cast<Coord>(rng()%1000000),static_cast<Coord>(rng()%1000000)});
  Tree tree(points,std::vector<int>(n,1));
  std::vector<Point> q;
  q.reserve(queries);
  for (int i=0;i<queries;++i)
    q.push_back({static_cast<Coord>(rng()%1000000),static_cast<Coord>(rng()%1000000)});
  std::size_t checksum=0;
  const auto start=std::chrono::steady_clock::now();
  for (auto p : q) checksum += tree.nearest(p)->index;
  const auto mid=std::chrono::steady_clock::now();
  for (auto p : q)
    tree.for_each_in_rect(p.x,p.x+1000,p.y,p.y+1000,[&](std::size_t id){ checksum+=id; });
  const auto end=std::chrono::steady_clock::now();
  std::cout << "n=" << n << " queries=" << queries
            << " nearest_ms=" << std::chrono::duration_cast<std::chrono::milliseconds>(mid-start).count()
            << " enumerate_ms=" << std::chrono::duration_cast<std::chrono::milliseconds>(end-mid).count()
            << " checksum=" << checksum << '\n';
}
