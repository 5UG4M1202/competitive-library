---
title: mydate
documentation_of: //library/misc/mydate.hpp
---

# mydate

Legacy date arithmetic using the proleptic Gregorian calendar. `mydate(y,m,d)` stores the supplied fields without validation; the default is `(0,0,0)`. Use `isValidDate` to check inputs. Comparisons are lexicographic, including invalid values. `daysInMonth` expects a month in 1..12 (other values return 31). `addDays`, `subDays`, subtraction and `dayOfWeek` use Julian day numbers; weekdays are Monday=0 through Sunday=6. `addMonths` clamps the day to the target month's end.

All operations are O(1), with no range endpoints or sentinel. Use valid dates and values whose intermediate integer operations and returned `int` year fit. In particular, `toJulianDay` calculates `year + 4800` in `int`; Julian conversions use truncating division and are not reliable over arbitrary negative dates. `subDays(LLONG_MIN)` overflows. `addMonths` with a negative total month can produce month 0 or negative because C++ division truncates toward zero. These legacy behaviors are preserved, not corrected.

Example:

```cpp
#include "library/misc/mydate.hpp"
#include <cassert>
int main() {
  mydate d(2024, 2, 28);
  assert(d.addDays(1) == mydate(2024, 2, 29));
  assert(d.addMonths(12) == mydate(2025, 2, 28));
}
```

The user reports creating legacy `@mydate` with AI assistance. Apache-2.0 applies to rrrriki's copyrightable human-authored contributions; this provenance report does not guarantee rights in every generated fragment. The original comment referring to Jean Meeus is preserved and does not establish a source-code license. The nonstandard `__isleap` dependency was replaced with an equivalent Gregorian leap-year expression.
