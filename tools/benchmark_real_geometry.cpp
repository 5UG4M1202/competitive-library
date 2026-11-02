// Synthetic performance probe; not a correctness test.
#include "../library/geometry/real_closest_pair.hpp"
#include "../library/geometry/real_farthest_pair.hpp"
#include <chrono>
#include <iostream>
#include <random>
int main() {
  std::mt19937 rng(103);
  std::vector<cp::geometry::RealPoint> p;
  for (int i=0;i<10000;++i)
    p.push_back({static_cast<double>(rng()%10000000)/8, static_cast<double>(rng()%10000000)/8});
  auto start=std::chrono::steady_clock::now();
  auto a=cp::geometry::closest_pair(p);
  auto middle=std::chrono::steady_clock::now();
  auto b=cp::geometry::farthest_pair(p);
  auto end=std::chrono::steady_clock::now();
  std::cout << "n=" << p.size()
            << " closest_ms=" << std::chrono::duration_cast<std::chrono::milliseconds>(middle-start).count()
            << " farthest_ms=" << std::chrono::duration_cast<std::chrono::milliseconds>(end-middle).count()
            << " check=" << (a && b) << '\n';
}
