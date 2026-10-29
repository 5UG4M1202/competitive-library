// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/data_structure/nqueen.hpp"
#include <algorithm>
#include <cassert>
#include <random>
#include <vector>

int main() {
  NQueen one(1);
  assert(one.solve() && one.is_valid() && one.size() == 1);
  NQueen impossible(2);
  assert(!impossible.solve() && impossible.get_queens().empty());
  NQueen four(4);
  assert(four.add_queen(0, 1));
  assert(four.add_queen(1, 3));
  assert(four.add_queen(2, 0));
  assert(four.add_queen(3, 2));
  // Regression: every row/column/diagonal is occupied, but (0,2) is empty.
  const auto before = four.get_queens();
  const auto added_before = four.get_added_queens();
  assert(!four.remove_queen(0, 2));
  assert(four.get_queens() == before);
  assert(four.get_added_queens() == added_before);
  assert(four.is_valid());
  for (int x = 0; x < 4; ++x)
    for (int y = 0; y < 4; ++y)
      assert(!four.is_valid(x, y));
  assert(four.remove_queen(0, 1));
  assert(!four.remove_queen(0, 1));
  assert(four.add_queen(0, 1));
  assert(four.get_queens() == before && four.is_valid());

  NQueen solved(4);
  assert(solved.solve());
  auto solved_before = solved.get_queens();
  auto solved_added = solved.get_added_queens();
  assert(!solved.remove_queen(0, 2));
  assert(solved.get_queens() == solved_before);
  assert(solved.get_added_queens() == solved_added);
  assert(solved.is_valid());

  std::mt19937 rng(20261029);
  for (int trial = 0; trial < 200; ++trial) {
    const int n = 1 + static_cast<int>(rng() % 8);
    NQueen board(n);
    std::vector<std::pair<int, int>> queens;
    for (int step = 0; step < 100; ++step) {
      int x = static_cast<int>(rng() % static_cast<unsigned>(n));
      int y = static_cast<int>(rng() % static_cast<unsigned>(n));
      bool valid = true;
      for (auto [a, b] : queens)
        if (a == x || b == y || a + b == x + y || a - b == x - y) valid = false;
      assert(board.is_valid(x, y) == valid);
      assert(board.add_queen(x, y) == valid);
      if (valid) queens.emplace_back(x, y);
      int rx = static_cast<int>(rng() % static_cast<unsigned>(n));
      int ry = static_cast<int>(rng() % static_cast<unsigned>(n));
      auto it = std::find(queens.begin(), queens.end(), std::pair{rx, ry});
      bool exists = it != queens.end();
      auto prior_added = board.get_added_queens();
      assert(board.remove_queen(rx, ry) == exists);
      assert(board.get_added_queens() == prior_added);
      if (exists) queens.erase(it);
      auto actual = board.get_queens();
      auto expected = queens;
      std::sort(expected.begin(), expected.end());
      assert(actual == expected);
      assert(board.is_valid() == (static_cast<int>(queens.size()) == n));
    }
  }
}
