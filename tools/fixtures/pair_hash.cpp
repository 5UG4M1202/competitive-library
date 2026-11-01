// @snippet-definition@
#include <cassert>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <utility>

int main() {
  using Pair = std::pair<long long, long long>;
  std::unordered_set<Pair, phash> seen;
  Pair a{std::numeric_limits<long long>::min(), std::numeric_limits<long long>::max()};
  assert(seen.insert(a).second);
  assert(!seen.insert(a).second && seen.count(a) == 1);
  for (long long x = -30; x <= 30; ++x)
    for (long long y = -30; y <= 30; ++y) seen.insert({x, y});
  assert(seen.size() == 61u * 61u + 1);
  assert(seen.count({17, -12}) == 1 && seen.count({31, 31}) == 0);
  std::unordered_map<Pair, int, phash> index;
  index[a] = 42;
  assert(index.at(a) == 42);
}
