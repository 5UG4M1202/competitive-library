// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/misc/mydate.hpp"
#include <cassert>
#include <chrono>
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
  // Characterization: negative total months use truncating division.
  assert(mydate(0, 1, 1).addMonths(-1) == mydate(0, 0, 1));
  std::mt19937 rng(12345);
  for (int i = 0; i < 10000; ++i) {
    int y = 1900 + int(rng() % 201);
    unsigned m = 1 + rng() % 12;
    unsigned d = 1 + rng() % unsigned(mydate::daysInMonth(y, int(m)));
    int offset = int(rng() % 2001) - 1000;
    std::chrono::sys_days base = std::chrono::year(y)/std::chrono::month(m)/std::chrono::day(d);
    std::chrono::year_month_day expected{base + std::chrono::days(offset)};
    mydate actual = mydate(y, int(m), int(d)).addDays(offset);
    assert(actual == mydate(int(expected.year()), int(unsigned(expected.month())), int(unsigned(expected.day()))));
    assert(actual.subDays(offset) == mydate(y, int(m), int(d)));
  }
}
