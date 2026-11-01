// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/misc/mydate.hpp"
#include <cassert>
#include <chrono>
#include <limits>
#include <random>

int main() {
  // Intended correctness: leap years, month-end clamping, weekday and arithmetic.
  assert(mydate::isValidDate(2000, 2, 29));
  assert(!mydate::isValidDate(1900, 2, 29));
  assert(!mydate::isValidDate(2024, 13, 1));
  assert(mydate(2024, 1, 31).addMonths(1) == mydate(2024, 2, 29));
  assert(mydate(2024, 2, 29).addDays(1) == mydate(2024, 3, 1));
  assert(mydate(2024, 1, 1).dayOfWeek() == 0);
  assert(mydate(2024, 3, 1) - mydate(2024, 2, 28) == 2);
  assert(mydate(0, 1, 1).addMonths(-1) == mydate(-1, 12, 1));
  assert(mydate(-1, 1, 31).addMonths(1) == mydate(-1, 2, 28));
  for (int y : {std::numeric_limits<int>::min(), -100000, -4801, -4800, -1,
                0, 1, 2000, 100000, std::numeric_limits<int>::max()}) {
    for (int m : {1, 2, 12}) {
      mydate d(y, m, mydate::daysInMonth(y, m));
      assert(mydate::fromJulianDay(mydate::toJulianDay(y, m, d.day)) == d);
    }
  }
  const int min_year = std::numeric_limits<int>::min();
  const int max_year = std::numeric_limits<int>::max();
  assert(mydate(min_year, 1, 1).addDays(1) == mydate(min_year, 1, 2));
  assert(mydate(max_year, 12, 31).subDays(1) == mydate(max_year, 12, 30));
  assert(mydate(min_year, 1, 31).addMonths(1) == mydate(min_year, 2, 29));
  assert(mydate(max_year, 12, 31).addMonths(-1) == mydate(max_year, 11, 30));
  std::mt19937 rng(12345);
  for (int i = 0; i < 10000; ++i) {
    int y = int(rng() % 40001) - 20000;
    unsigned m = 1 + rng() % 12;
    unsigned d = 1 + rng() % unsigned(mydate::daysInMonth(y, int(m)));
    int offset = int(rng() % 2001) - 1000;
    std::chrono::sys_days base = std::chrono::year(y)/std::chrono::month(m)/std::chrono::day(d);
    assert(mydate::toJulianDay(y, int(m), int(d)) == base.time_since_epoch().count() + 2440588LL);
    std::chrono::year_month_day expected{base + std::chrono::days(offset)};
    mydate actual = mydate(y, int(m), int(d)).addDays(offset);
    assert(actual == mydate(int(expected.year()), int(unsigned(expected.month())), int(unsigned(expected.day()))));
    assert(actual.subDays(offset) == mydate(y, int(m), int(d)));
  }
}
