#include <iostream>
#include <vector>
int main() {
  std::vector<long long> a{3, 1, 2, 1};
  std::cout << cp::inversion_count(a) << '\n';  // 4
  auto ending = cp::inversion_count_ending_at(a);
  auto starting = cp::inversion_count_starting_at(a);
  for (std::size_t i = 0; i < a.size(); ++i) std::cout << (i ? " " : "") << ending[i];
  std::cout << '\n';  // 0 1 1 2
  for (std::size_t i = 0; i < a.size(); ++i) std::cout << (i ? " " : "") << starting[i];
  std::cout << '\n';  // 3 0 1 0
}
