---
title: DistinctKSegments
documentation_of: //library/data_structure/distinct_k_segments.hpp
---

`DistinctKSegments<Int>(a, K).solve()` returns the maximum sum of distinct
counts over K nonempty contiguous segments of `a`. For nonempty input K is
clamped to [1, N]; for empty input it becomes 0 and the result is 0.
Values must be nonnegative integers representable as `int`; compression to
0..M-1 avoids excessive memory. N and maximum value must fit in `int` with
room for N+1 and max+1. Arithmetic uses `long long`; allocations must fit.
The internal segment-tree ranges are half-open. Time is O(K N log N), and
memory O(K N + M). AtCoder Library's `<atcoder/lazysegtree>` is required.

`solve()` mutates state and is intended to be called only once per instance;
repeated calls are not reset and may return incorrect results. Segment-tree
identity 0 is valid only for this algorithm's nonnegative values. Input outside
the preconditions may cause out-of-bounds access or overflow.

The user reports creating legacy `@DistinctMax` with AI assistance. The snippet
refers to https://atcoder.jp/contests/abc397/submissions/71461138; this link is
retained as a reference, not proof of a license. Apache-2.0 applies to rrrriki's
copyrightable human-authored contributions; this provenance report does not
guarantee rights in every generated fragment. ACL remains an external CC0 dependency.

```cpp
#include <iostream>
#include <vector>
#include "library/data_structure/distinct_k_segments.hpp"
int main() {
  DistinctKSegments solver(std::vector<int>{0, 1, 0}, 2);
  std::cout << solver.solve() << '\n'; // 3
}
```
