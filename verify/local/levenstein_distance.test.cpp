// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/string/levenstein_distance.hpp"
#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

int reference(const std::string& a, const std::string& b, int i, int j) {
  if (i == static_cast<int>(a.size())) return static_cast<int>(b.size()) - j;
  if (j == static_cast<int>(b.size())) return static_cast<int>(a.size()) - i;
  return std::min({1 + reference(a, b, i + 1, j),
                   1 + reference(a, b, i, j + 1),
                   (a[i] != b[j]) + reference(a, b, i + 1, j + 1)});
}

int main() {
  assert(levenstein_distance("kitten", "sitting") == 3);
  assert(levenstein_distance("", "") == 0);
  std::vector<std::string> words{""};
  for (int n = 1; n <= 4; ++n) {
    for (int mask = 0; mask < (1 << n); ++mask) {
      std::string s(n, 'a');
      for (int i = 0; i < n; ++i) if ((mask >> i) & 1) s[i] = 'b';
      words.push_back(s);
    }
  }
  for (const auto& a : words) for (const auto& b : words) {
    assert(levenstein_distance(a, b) == reference(a, b, 0, 0));
  }
}
