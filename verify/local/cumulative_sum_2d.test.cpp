// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/data_structure/cumulative_sum_2d.hpp"
#include <cassert>
#include <random>
#include <vector>

int main() {
  CumulativeSum2D<long long> empty(0, 0);
  empty.build();
  assert(empty.query(0, 0, 0, 0) == 0);
  std::mt19937 rng(7331);
  for (int h = 1; h <= 7; ++h) for (int w = 1; w <= 8; ++w) {
    CumulativeSum2D<long long> sum(h, w);
    std::vector<std::vector<long long>> grid(h, std::vector<long long>(w));
    for (int i = 0; i < 80; ++i) {
      int x = static_cast<int>(rng() % w), y = static_cast<int>(rng() % h);
      int delta = static_cast<int>(rng() % 19) - 9;
      sum.add(x, y, delta);
      grid[y][x] += delta;
    }
    sum.build();
    for (int sy = 0; sy <= h; ++sy) for (int gy = sy; gy <= h; ++gy)
      for (int sx = 0; sx <= w; ++sx) for (int gx = sx; gx <= w; ++gx) {
        long long expected = 0;
        for (int y = sy; y < gy; ++y) for (int x = sx; x < gx; ++x)
          expected += grid[y][x];
        assert(sum.query(sx, sy, gx, gy) == expected);
      }
  }
}
