// @snippet-definition@
#include <cassert>
#include <cstdint>
#include <limits>
#include <random>
#include <string>

int main() {
  auto check = [](std::uint64_t x) { assert(to_digit(x) == static_cast<int>(std::to_string(x).size())); };
  check(0);
  check(std::numeric_limits<std::uint64_t>::max());
  std::uint64_t power = 1;
  for (int i = 0; i <= 19; ++i) {
    check(power - 1);
    check(power);
    check(power + 1);
    if (i < 19) power *= 10;
  }
  for (int bit = 0; bit < 64; ++bit) {
    auto x = std::uint64_t{1} << bit;
    check(x - 1);
    check(x);
    check(x + 1);
  }
  std::mt19937_64 rng(314159);
  for (int i = 0; i < 100000; ++i) check(rng());
}
