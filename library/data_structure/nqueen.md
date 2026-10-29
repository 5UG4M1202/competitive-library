---
title: NQueen
documentation_of: //library/data_structure/nqueen.hpp
---

Legacy `@NQUEEN` extraction. The user reports writing it with AI assistance.
Apache-2.0 applies to rrrriki's copyrightable human-authored contributions;
this provenance report does not guarantee rights in every generated fragment.

```cpp
#include "library/data_structure/nqueen.hpp"
int main() {
  NQueen board(4);
  if (!board.solve() || !board.is_valid()) return 1;
  board.print();
}
```

This prints a valid four-queen board, one row per line.

`NQueen(n)` requires `n > 0`. Coordinates are zero-based and must lie in
`[0,n)` for additions and removals; `x+y` must fit in `int`. `is_valid(x,y)`
tests whether a square is free from attacks, while `is_valid()` checks whether
the row set has `n` entries. `solve(x)` recursively fills rows beginning at
`x`, skipping occupied rows; use `0 <= x <= n`. Successful placements remain
on the board and are listed by `get_added_queens()`. The getters return sorted
coordinates. `print` writes to standard output; streaming writes coordinates.

Hash-set operations are expected O(1); ordered queen-set operations cost
O(log n). Backtracking can take O(n!) time and O(n) recursion depth. Printing
uses O(n²) time and memory. There are no sentinels or external dependencies.

`remove_queen(x, y)` returns `false` without changing the board when that
square contains no queen; otherwise it removes the queen and returns `true`.
The former independent row/column/diagonal checks could falsely accept an
empty square and corrupt the board. A regression test covers that case, and a
deterministic randomized test compares addition, removal, and attack checks
against a simple model. Removing from the ordered set costs O(log n), while
the accompanying hash-set operations are expected O(1).
