// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index

#include <cassert>
#include <tuple>
#include <vector>

#include "../../library/data_structure/interval_set.hpp"

int main() {
  constexpr int identity = -1;
  IntervalSet<int, int> intervals(identity);

  assert(intervals.size() == 0);
  assert(intervals.covered_all() == 0);
  assert(intervals.get_val(3) == identity);
  assert(intervals.covered(4, 4));

  intervals.update(2, 5, 7);
  intervals.update(5, 8, 7);
  assert(intervals.size() == 1);
  assert(intervals.covered(2, 8));
  assert(intervals.covered_all() == 6);

  intervals.update(3, 6, 9);
  assert(intervals.size() == 3);
  assert(intervals.get_val(2) == 7);
  assert(intervals.get_val(3) == 9);
  assert(intervals.get_val(6) == 7);
  assert(!intervals.covered(2, 8));
  assert(intervals.covered_length_in(0, 10) == 6);
  assert(intervals.contained_intervals_in(1, 8) == 3);
  // Characterization, not intended correctness: the mathematical answer is
  // 3, but legacy contained_intervals_in skips the node starting at L.
  assert(intervals.contained_intervals_in(2, 8) == 2);

  intervals.erase(4, 7);
  assert(intervals.size() == 3);
  assert(intervals.covered_all() == 3);
  assert(intervals.get_val(3) == 9);
  assert(intervals.get_val(4) == identity);
  assert(intervals.get_val(7) == 7);
  assert(intervals.covered_length_in(3, 8) == 2);

  std::vector<int> initial{1, 1, 2, 2, 2, 1};
  IntervalSet<int, int> from_vector(initial, identity);
  assert(from_vector.size() == 3);
  assert(from_vector.covered_all() == 6);
  assert(from_vector[0] == 1);
  assert(from_vector[2] == 2);
  assert(from_vector[5] == 1);

  IntervalSet<int, int> occupancy(identity);
  occupancy.insert(0, 2);
  occupancy.insert(2, 4);
  assert(occupancy.size() == 1);
  assert(occupancy.get_mex(0) == 4);
  assert(occupancy.get_mex(5) == 5);

  IntervalSet<int, int> adjacent_values(identity);
  adjacent_values.update(0, 1, 1);
  adjacent_values.update(1, 2, 2);
  // Characterization: the mathematical mex is 2, but legacy get_mex only
  // advances through the first interval.
  assert(adjacent_values.get_mex(0) == 1);

  IntervalSet<int, int> negative_endpoints(identity);
  negative_endpoints.update(-10, -5, 1);
  negative_endpoints.update(-10, -7, 2);
  std::vector<std::tuple<int, int, int>> negative_nodes;
  for (const auto& node : negative_endpoints) {
    negative_nodes.emplace_back(node.l, node.r, node.val);
  }
  // Characterization: a zero-length stale interval remains because the
  // mutation lookup uses T{} (zero) as a right-end sentinel.
  assert((negative_nodes ==
          std::vector<std::tuple<int, int, int>>{
              {-10, -10, 1}, {-10, -7, 2}, {-7, -5, 1}}));

  long long weighted = 0;
  auto add = [&](int l, int r, int value) {
    weighted += static_cast<long long>(r - l) * value;
  };
  auto del = [&](int l, int r, int value) {
    weighted -= static_cast<long long>(r - l) * value;
  };
  IntervalSet<int, int> observed(identity);
  observed.update(10, 13, 2, add, del);
  observed.update(11, 12, 5, add, del);
  assert(weighted == 9);
  observed.erase(10, 12, add, del);
  assert(weighted == 2);

  std::vector<std::tuple<int, int, int>> nodes;
  for (const auto& node : observed) {
    nodes.emplace_back(node.l, node.r, node.val);
  }
  assert((nodes == std::vector<std::tuple<int, int, int>>{{12, 13, 2}}));
  return 0;
}
