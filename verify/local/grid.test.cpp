// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/graph/grid.hpp"
#include <algorithm>
#include <cassert>
#include <random>
#include <sstream>
#include <string>
#include <vector>

int main() {
  GRID<> initial;
  assert(initial.H == 0 && initial.W == 0);
  GRID<> input(1, 1);
  input.wall('!');
  std::istringstream in("abc\ndef\n");
  auto* oldin = std::cin.rdbuf(in.rdbuf());
  input.read(1, 3);
  input.read(1, 3);
  std::cin.rdbuf(oldin);
  assert(input.H == 1 && input.W == 3);
  assert(input[0][0] == 'd' && input[0][2] == 'f');
  assert(input.wall_set.count('!') && input.wall_set.count('#'));

  GRID<> blocked(2, 3, '.');
  blocked.wall('#');
  blocked[0][0] = '#';
  auto wall_distance = blocked.bfs(0, 0);
  auto invalid_distance = blocked.bfs(-1, 0);
  for (const auto& row : wall_distance) for (auto x : row) assert(x == GRID<>::INF_VALUE);
  for (const auto& row : invalid_distance) for (auto x : row) assert(x == GRID<>::INF_VALUE);
  blocked.bfs_vis(0, 0);
  assert(!blocked.vis[0][0]);

  GRID<> small(2, 3, '.');
  small[0][0] = 'S'; small[1][2] = 'G';
  auto endpoints = small.get_start_goal();
  assert((endpoints.first == std::pair<int, int>(0, 0)));
  assert((endpoints.second == std::pair<int, int>(1, 2)));
  assert(small.get_positions('.').size() == 4);
  GRID<> no_endpoints(1, 1, '.');
  auto missing = no_endpoints.get_start_goal();
  assert((missing.first == std::pair<int, int>(0, 0)));
  assert((missing.second == std::pair<int, int>(0, 0)));

  std::mt19937 rng(97831);
  for (int trial = 0; trial < 150; ++trial) {
    int h = static_cast<int>(rng() % 5) + 1, w = static_cast<int>(rng() % 5) + 1;
    GRID<> g(h, w, '.');
    g.wall('#');
    for (int i = 0; i < h; ++i) for (int j = 0; j < w; ++j)
      if (rng() % 4 == 0) g[i][j] = '#';
    int si = static_cast<int>(rng() % h), sj = static_cast<int>(rng() % w);
    std::vector<std::vector<long long>> expected(h, std::vector<long long>(w, GRID<>::INF_VALUE));
    if (g[si][sj] != '#') expected[si][sj] = 0;
    for (int step = 0; step < h * w; ++step) {
      auto prev = expected;
      for (int i = 0; i < h; ++i) for (int j = 0; j < w; ++j) {
        if (g[i][j] == '#') continue;
        for (auto [di, dj] : g.directions) {
          int ni = i + di, nj = j + dj;
          if (ni < 0 || ni >= h || nj < 0 || nj >= w || prev[ni][nj] == GRID<>::INF_VALUE) continue;
          expected[i][j] = std::min(expected[i][j], prev[ni][nj] + 1);
        }
      }
    }
    assert(g.bfs(si, sj) == expected);
    g.bfs_vis(si, sj);
    for (int i = 0; i < h; ++i) for (int j = 0; j < w; ++j)
      assert(g.vis[i][j] == (expected[i][j] != GRID<>::INF_VALUE));
  }
}
