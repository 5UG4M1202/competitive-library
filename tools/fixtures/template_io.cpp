// @snippet-definition@
#include <array>
#include <cassert>
#include <deque>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace sample {
struct Value { int x; };
std::istream& operator>>(std::istream& stream, Value& value) { return stream >> value.x; }
}

int main() {
  std::vector<std::pair<int, int>> pairs(2);
  std::deque<std::vector<long long>> nested(2, std::vector<long long>(2));
  std::array<std::pair<int, std::vector<int>>, 2> composite{{{0, std::vector<int>(2)}, {0, std::vector<int>(2)}}};
  int raw[2]{};
  std::vector<sample::Value> custom(2);
  std::vector<bool> bits(3);
  std::istringstream input("1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 0 7 1");
  auto* old_in = std::cin.rdbuf(input.rdbuf());
  in(pairs, nested, composite, raw, custom, bits);
  assert(pairs[0] == std::make_pair(1, 2) && pairs[1] == std::make_pair(3, 4));
  assert(nested[0][0] == 5 && nested[1][1] == 8);
  assert(composite[0].first == 9 && composite[0].second[1] == 11);
  assert(composite[1].first == 12 && composite[1].second[1] == 14);
  assert(raw[0] == 15 && raw[1] == 16 && custom[0].x == 17 && custom[1].x == 18);
  assert(!bits[0] && bits[1] && bits[2]);
  std::istringstream incomplete("1");
  std::cin.rdbuf(incomplete.rdbuf());
  std::vector<bool> partial{false, true, false};
  scan(partial);
  assert(partial[0] && partial[1] && !partial[2] && std::cin.fail());
  std::cin.rdbuf(old_in);
  std::cin.clear();

  std::ostringstream output;
  auto* old_out = std::cout.rdbuf(output.rdbuf());
  std::vector<int> numbers{1, 2, 3};
  const std::vector<int> empty;
  out_c(numbers, ",");
  out_c(empty);
  out_c_pairs(pairs, ":", "|");
  std::cout.rdbuf(old_out);
  assert(output.str() == "1,2,3\n\n1:2|3:4\n");
}
