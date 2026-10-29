---
title: Third-party notices
layout: page
---

# Third-party notices

This file records known third-party origins, licenses, and unresolved
provenance. It is informational: it does not replace or add conditions to the
licenses identified below.

## Included or adapted material

### drken1215/algorithm — IntervalSet

- Comparison source reviewed: <https://github.com/drken1215/algorithm/blob/79b5a6a826690c4d8e0bd64e812e85b5f425ea6b/DataStructureBasic/intervals_set.cpp>
- Upstream repository: <https://github.com/drken1215/algorithm>
- License: CC0 1.0 Universal
- License evidence at the reviewed commit: <https://github.com/drken1215/algorithm/blob/79b5a6a826690c4d8e0bd64e812e85b5f425ea6b/LICENSE>
- Historical evidence: the repository LICENSE was
  [added in 2020](https://github.com/drken1215/algorithm/commit/07b03604bacedef00e847264de8ef2c3e047d119),
  before the tracked IntervalSet history examined in 2023–2026.

`library/data_structure/interval_set.hpp` contains an adapted IntervalSet core.
Distinctive common elements include the `Node` representation, vector
constructor, lookup and mex operations, callback-based update/erase API, and
the split/merge sequence using `std::set::emplace_hint`.

The permalink above is the comparison version reviewed for this notice; it is
not a claim that this exact revision was the one originally referenced. Local
adaptations include standalone standard-library includes, `std::`
qualification, const overloads, `total_cover`, `covered_all`,
`covered_length_in`, `contained_intervals_in`, documentation, and verification
tests. The exact historical upstream revision used by the original snippet is
not established. Similarity is evidence of provenance, not by itself a claim
about authorship.

The upstream portions remain available under CC0 1.0 Universal. Apache-2.0 in
this repository applies only to rrrriki's copyrightable additions and
modifications; it does not withdraw or narrow the upstream CC0 rights.

### NyaanNyaan/library — LazyReversibleRBST

- Reviewed source version: commit [`b3981adc80a800b2584980b01821324ea6c77183`](https://github.com/NyaanNyaan/library/tree/b3981adc80a800b2584980b01821324ea6c77183/rbst)
- Upstream files: [`rbst-base.hpp`](https://github.com/NyaanNyaan/library/blob/b3981adc80a800b2584980b01821324ea6c77183/rbst/rbst-base.hpp) and [`lazy-reversible-rbst.hpp`](https://github.com/NyaanNyaan/library/blob/b3981adc80a800b2584980b01821324ea6c77183/rbst/lazy-reversible-rbst.hpp)
- License: CC0 1.0 Universal
- License evidence: <https://github.com/NyaanNyaan/library/blob/b3981adc80a800b2584980b01821324ea6c77183/LICENSE>

`library/data_structure/lazy_reversible_rbst.hpp` and the maintained `@RBST`
snippet contain the upstream randomized BST base, lazy/reversible node, and
range-operation implementation. Local adaptations include standalone standard
library includes, `std::` qualification, provenance comments, documentation,
verification tests, and synchronization checks.

The reviewed source is structurally the same implementation recorded by the
legacy snippet. The exact historical revision originally copied into the
snippet is not established. The upstream portions remain available under CC0
1.0 Universal; Apache-2.0 applies only to rrrriki's copyrightable additions and
modifications.

### competitive-verifier 4.2.2 — generated documentation resources

- Project: <https://github.com/competitive-verifier/competitive-verifier>
- Version used for local validation and CI configuration: 4.2.2
- License: MIT
- License evidence: <https://github.com/competitive-verifier/competitive-verifier/blob/v4.2.2/LICENSE>

The verifier is a development dependency and is not vendored in this
repository. Its documentation command copies Jekyll templates and resources
into generated site source. The generation scripts copy this notice and the
project license files into that output. The applicable MIT text is reproduced
below so redistributions of those generated resources retain the required
notice.

> MIT License
>
> Copyright (c) 2022 kzrnm
>
> Permission is hereby granted, free of charge, to any person obtaining a copy
> of this software and associated documentation files (the "Software"), to deal
> in the Software without restriction, including without limitation the rights
> to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
> copies of the Software, and to permit persons to whom the Software is
> furnished to do so, subject to the following conditions:
>
> The above copyright notice and this permission notice shall be included in
> all copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
> IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
> FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
> AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
> LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
> OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
> SOFTWARE.

## User-reported provenance

### mydate, DistinctKSegments, and NQueen

The user reports creating `@mydate`, `@DistinctMax`, and `@NQUEEN` with AI
assistance. Apache-2.0 applies to rrrriki's copyrightable human-authored
contributions; the report is not a warranty that generated fragments are
copyrightable or free of third-party claims. The legacy `@mydate` comment
refers to Jean Meeus's calendar algorithms; `@DistinctMax` records the
reference <https://atcoder.jp/contests/abc397/submissions/71461138> and depends
on external AtCoder Library (CC0-1.0). These references alone do not establish
a license for every implementation fragment. `@NQUEEN` names no concrete
external implementation source in the legacy snippet.

### EulerTour

- Legacy snippet author tag: `@rrrrikiOW`
- User report: written by the user with GitHub Copilot assistance
- Concrete external implementation source named by the snippet: none
- External build dependency: [AtCoder Library](https://github.com/atcoder/ac-library), CC0 1.0 Universal
- ACL license evidence: <https://github.com/atcoder/ac-library/blob/v1.6/LICENSE>

`library/graph/euler_tour.hpp` and the maintained `@EulerT` snippet preserve
the existing API while adding standalone includes, namespace qualification,
documentation, verification, and synchronization checks. The repository does
not vendor ACL source; local validation used an existing ACL checkout of
unknown exact revision, while the draft CI configuration pins release v1.6.

This is a record of the user's provenance statement, not a certification that
all generated fragments are copyrightable or free of third-party claims.
Apache-2.0 applies only to rrrriki's copyrightable human-authored
contributions. GitHub Copilot use alone is not treated as proof of complete
ownership or provenance.

## Build and include dependencies not vendored here

These entries describe dependencies or references; they are not claims that
their source code is copied into the split IntervalSet header.

- [AtCoder Library](https://github.com/atcoder/ac-library), CC0 1.0 Universal:
  required by the maintained EulerTour and referenced by some unresolved
  snippets in `cpp.json`; no ACL source is vendored in the split library.
- [jekyll-theme-minimal](https://github.com/pages-themes/minimal), CC0 1.0
  Universal: selected by the documentation build environment; it is not
  vendored in this repository.

## Legacy snippets with incomplete provenance

`cpp.json` is a current snippet collection with mixed provenance. The
maintained `@interval_set` entry is synchronized from
`library/data_structure/interval_set.hpp`: its adapted upstream core remains
CC0 1.0 Universal, while Apache-2.0 applies only to rrrriki's copyrightable
additions and modifications. Its source and split-license notice must remain in
the snippet body.

The maintained `@RBST` entry is synchronized from
`library/data_structure/lazy_reversible_rbst.hpp`: its NyaanNyaan/library core
also remains CC0 1.0 Universal, while Apache-2.0 applies only to rrrriki's
copyrightable additions and modifications. Its source and split-license notice
must remain in the snippet body.

The maintained `@EulerT` entry is synchronized from
`library/graph/euler_tour.hpp` and retains the user-reported Copilot-assisted
authorship statement, the absence of a concrete third-party implementation
source in the legacy snippet, and the external ACL dependency notice.

The other 17 snippets have not all been cleared for relicensing and remain
excluded from the repository's blanket Apache-2.0 grant. Partial evidence
currently includes:

- Rolling-hash comments refer to a
  [Qiita article](https://qiita.com/keymoon/items/11fac5627672a6d6a9f6), while
  [ei1333's library](https://github.com/ei1333/library/blob/master/LICENSE) uses
  the Unlicense. The exact origin and reuse permission for the legacy C++ body
  remain unresolved; an article URL or another implementation's license is not
  sufficient evidence by itself.
- Several snippets contain AtCoder submission links or author handles, while
  other sections contain no source URL. A public submission or an unmarked
  section does not by itself establish authorship or relicensing authority.

These observations are not a complete file-level license map and do not
establish that unmarked sections are original or freely licensed.

Review and record each snippet separately before changing the license scope or
redistributing the snapshot as a licensed collection.
