// @snippet-definition@
#include <cassert>
#include <limits>

namespace custom {
struct Number { int v; };
Number operator-(Number a, Number b) { return {a.v - b.v}; }
Number operator+(Number a, Number b) { return {a.v + b.v}; }
Number abs(Number a) { return {a.v < 0 ? -a.v : a.v}; }
}

int main() {
  assert(mnht(0, 0, 0, 0) == 0);
  assert(mnht(-2, 3, 4, -5) == 14);
  assert(mnht(std::numeric_limits<long long>::max(), 0LL, 0LL, 0LL) == std::numeric_limits<long long>::max());
  assert(mnht(0.5, -0.5, 1.0, 1.0) == 2.0);
  assert(mnht(custom::Number{1}, custom::Number{2}, custom::Number{4}, custom::Number{6}).v == 7);
  for (int a = -10; a <= 10; ++a)
    for (int b = -10; b <= 10; ++b)
      for (int c = -10; c <= 10; ++c) {
        int d = a - b;
        int dx = a >= c ? a - c : c - a;
        int dy = b >= d ? b - d : d - b;
        assert(mnht(a, b, c, d) == dx + dy);
        assert(mnht(a, b, c, d) == mnht(c, d, a, b));
      }
}
