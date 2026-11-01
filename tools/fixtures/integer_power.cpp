// @snippet-definition@
#include <cassert>
#include <limits>

int main() {
  assert(_pow(0, 0) == 1 && _pow(0, 4) == 0);
  assert(_pow(-2, 63) == std::numeric_limits<long long>::min());
  assert(_pow(2, 62) == (1LL << 62));
  assert(_pow(-1, std::numeric_limits<long long>::max()) == -1);
  for (long long x = -10; x <= 10; ++x) {
    long long expected = 1;
    for (long long n = 0; n <= 10; ++n) {
      assert(_pow(x, n) == expected);
      expected *= x;
    }
  }
}
