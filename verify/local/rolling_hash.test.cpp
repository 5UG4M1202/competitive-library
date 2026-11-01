// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/string/rolling_hash.hpp"
#include <cassert>
#include <random>
#include <string>
#include <vector>

int main() {
  RollingHash rh(1000003);
  auto empty = rh.build(std::string());
  assert(rh.query(empty, 0, 0) == 0);
  std::string bytes{"\x80\xff\0z", 4};
  auto b = rh.build(bytes);
  std::vector<int> unsigned_bytes{128, 255, 0, 122};
  assert(b == rh.build(unsigned_bytes));
  assert(rh.query(b, 0, 1) == 128);
  std::mt19937 rng(271828);
  for (int trial = 0; trial < 300; ++trial) {
    int n = static_cast<int>(rng() % 30);
    std::string s;
    for (int i = 0; i < n; ++i) s.push_back(static_cast<char>(rng() % 256));
    auto hashed = rh.build(s);
    for (int l = 0; l <= n; ++l) for (int r = l; r <= n; ++r) {
      auto sub = rh.build(s.substr(l, r - l));
      assert(rh.query(hashed, l, r) == sub.back());
      int middle = l + (r - l) / 2;
      assert(rh.combine(rh.query(hashed, l, middle), rh.query(hashed, middle, r),
                        static_cast<std::size_t>(r - middle)) == sub.back());
    }
    for (int i = 0; i < 20; ++i) {
      int a = static_cast<int>(rng() % (n + 1));
      int c = static_cast<int>(rng() % (n + 1));
      int length = 0;
      while (a + length < n && c + length < n && s[a + length] == s[c + length]) ++length;
      assert(rh.lcp(hashed, a, n, hashed, c, n) == length);
    }
  }
}
