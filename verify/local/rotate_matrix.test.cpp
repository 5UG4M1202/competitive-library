// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/misc/rotate_matrix.hpp"
#include <cassert>
#include <string>
#include <vector>

int main() {
  assert((rotate_matrix(std::vector<std::string>{"abc", "def"}) ==
          std::vector<std::string>{"da", "eb", "fc"}));
  assert((rotate_matrix(std::vector<std::vector<int>>{{1, 2, 3}, {4, 5, 6}}) ==
          std::vector<std::vector<int>>{{4, 1}, {5, 2}, {6, 3}}));
  assert(rotate_matrix(std::vector<std::string>{""}).empty());
  for (int h = 1; h <= 8; ++h) for (int w = 1; w <= 8; ++w) {
    std::vector<std::vector<int>> a(h, std::vector<int>(w));
    for (int i = 0; i < h; ++i) for (int j = 0; j < w; ++j) a[i][j] = i * w + j;
    auto b = rotate_matrix(a);
    assert(static_cast<int>(b.size()) == w && static_cast<int>(b[0].size()) == h);
    for (int i = 0; i < h; ++i) for (int j = 0; j < w; ++j)
      assert(b[j][h - 1 - i] == a[i][j]);
    assert(rotate_matrix(rotate_matrix(rotate_matrix(b))) == a);
  }
}
