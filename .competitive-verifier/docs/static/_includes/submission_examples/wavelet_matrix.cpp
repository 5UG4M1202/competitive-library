#include <cassert>
#include <vector>
int main() {
  cp::WaveletMatrix<int> wm(std::vector<int>{5, 1, 5, -2, 3});
  assert(wm.kth(1, 5, 1) == 1); // [-2,1,3,5] の 0 始まり 1 番目
  assert(wm.count(0, 5, 5) == 2);
  assert(wm.range_count(0, 5, 1, 5) == 2); // 1 以上 5 未満
  assert(wm.predecessor(0, 5, 1).value() == -2);
  assert(wm.lower_bound(0, 5, 2).value() == 3);
}
