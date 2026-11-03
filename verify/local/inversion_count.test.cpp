// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/data_structure/inversion_count.hpp"
#include <cassert>
#include <cstdlib>
#include <functional>
#include <limits>
#include <random>
#include <string>
#include <vector>

int main() {
  assert(cp::inversion_count(std::vector<int>{}) == 0);
  assert(cp::inversion_count(std::vector<int>{3, 1, 2, 1}) == 4);
  assert((cp::inversion_count_ending_at(std::vector<int>{3, 1, 2, 1}) == std::vector<long long>{0, 1, 1, 2}));
  assert((cp::inversion_count_starting_at(std::vector<int>{3, 1, 2, 1}) == std::vector<long long>{3, 0, 1, 0}));
  assert(cp::inversion_count_ending_at(std::vector<int>{}).empty());
  assert(cp::inversion_count_starting_at(std::vector<int>{}).empty());
  assert(cp::inversion_count(std::vector<int>{5, 5, 5}) == 0);
  assert(cp::inversion_count(std::vector<int>{1, 2, 3}, std::greater<>{}) == 3);
  assert(cp::inversion_count(std::vector<long long>{
      std::numeric_limits<long long>::max(), std::numeric_limits<long long>::min()}) == 1);
  assert(cp::inversion_count(std::vector<std::string>{"z", "a", "z"}) == 1);
  std::vector<int> descending(100000);
  for (int i = 0; i < 100000; ++i) descending[i] = 100000 - i;
  assert(cp::inversion_count(descending) == 4999950000LL);
  assert(cp::inversion_count_ending_at(descending).back() == 99999);
  assert(cp::inversion_count_starting_at(descending).front() == 99999);
  std::mt19937 gen(7493);
  for (int rep = 0; rep < 1000; ++rep) {
    const int n = static_cast<int>(gen() % 70);
    std::vector<int> a(static_cast<std::size_t>(n));
    for (auto& x : a) x = static_cast<int>(gen() % 11) - 5;
    const auto original = a;
    long long expected = 0, reverse = 0;
    std::vector<long long> ending(n), starting(n), reverse_ending(n), reverse_starting(n), abs_ending(n), abs_starting(n);
    for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j) {
      expected += a[i] > a[j];
      reverse += a[i] < a[j];
      if (a[i] > a[j]) { ++starting[i]; ++ending[j]; }
      if (a[i] < a[j]) { ++reverse_starting[i]; ++reverse_ending[j]; }
      if (std::abs(a[i]) > std::abs(a[j])) { ++abs_starting[i]; ++abs_ending[j]; }
    }
    assert(cp::inversion_count(a) == expected);
    assert(cp::inversion_count(a, std::greater<>{}) == reverse);
    assert(cp::inversion_count_ending_at(a) == ending);
    assert(cp::inversion_count_starting_at(a) == starting);
    assert(cp::inversion_count_ending_at(a, std::greater<>{}) == reverse_ending);
    assert(cp::inversion_count_starting_at(a, std::greater<>{}) == reverse_starting);
    const auto absolute_less = [](int x, int y) { return std::abs(x) < std::abs(y); };
    assert(cp::inversion_count_ending_at(a, absolute_less) == abs_ending);
    assert(cp::inversion_count_starting_at(a, absolute_less) == abs_starting);
    assert(a == original);
  }
}
