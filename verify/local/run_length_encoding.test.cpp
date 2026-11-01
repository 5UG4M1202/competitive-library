// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/string/run_length_encoding.hpp"
#include <cassert>
#include <random>
#include <string>
#include <vector>

int main() {
  assert(rle(std::string()).empty());
  assert(rle(std::vector<int>()).empty());
  auto sample = rle(std::string("aaabbc"));
  assert(sample.size() == 3 && sample[0] == std::make_pair('a', 3));
  std::mt19937 rng(8128);
  for (int n = 0; n < 100; ++n) for (int t = 0; t < 50; ++t) {
    std::vector<int> values(n);
    for (auto &v : values) v = static_cast<int>(rng() % 5);
    auto runs = rle(values);
    std::vector<int> restored;
    for (std::size_t i = 0; i < runs.size(); ++i) {
      assert(runs[i].second > 0);
      if (i) assert(runs[i - 1].first != runs[i].first);
      for (int j = 0; j < runs[i].second; ++j) restored.push_back(runs[i].first);
    }
    assert(restored == values);
  }
}
