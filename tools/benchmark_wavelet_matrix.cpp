#include "../library/data_structure/wavelet_matrix.hpp"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>
int main() {
 constexpr std::size_t n=200000,q=200000;
 std::mt19937 rng(20261103); std::vector<int> a(n);
 for (auto& x:a) x=static_cast<int>(rng() & 0x7fffffffU);
 std::vector<std::size_t> l(q),r(q),k(q);
 for (std::size_t i=0;i<q;++i) { auto x=rng()%n,y=rng()%n; if(x>y)std::swap(x,y); l[i]=x;r[i]=y+1;k[i]=rng()%(r[i]-l[i]); }
 const auto start=std::chrono::steady_clock::now();
 cp::WaveletMatrix<int> wm(a);
 const auto built=std::chrono::steady_clock::now();
 std::uint64_t sum=0; for(std::size_t i=0;i<q;++i) sum+=static_cast<unsigned>(wm.kth(l[i],r[i],k[i]));
 const auto done=std::chrono::steady_clock::now();
 std::cout << "checksum=" << sum << " build_ms=" << std::chrono::duration_cast<std::chrono::milliseconds>(built-start).count() << " query_ms=" << std::chrono::duration_cast<std::chrono::milliseconds>(done-built).count() << '\n';
}