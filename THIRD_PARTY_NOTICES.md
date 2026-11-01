---
title: Third-party notices
layout: page
---

# Third-party notices

This file records known third-party origins, licenses, and unresolved
provenance. It is informational: it does not replace or add conditions to the
licenses identified below.

## Included or adapted material

### ei1333/library — graph template comparison

- Reviewed source: [`graph/graph-template.hpp` at `088b4c8`](https://github.com/ei1333/library/blob/088b4c8b49f85194099305d998e7113dc3d5dd2e/graph/graph-template.hpp)
- Reviewed license: [Unlicense at the same revision](https://github.com/ei1333/library/blob/088b4c8b49f85194099305d998e7113dc3d5dd2e/LICENSE)

The legacy `Edge`, `Graph`, and `Edges` definitions in `cpptemplate` closely
match the reviewed implementation, including adjacency storage, edge numbering,
and input handling. The local version additionally defines edge comparison
and output operators. Separation adds standard headers and namespace
qualification, and initializes the default graph's edge count. Similarity is
evidence of possible adaptation, not proof of the exact historical source or
revision. Upstream rights remain available under the Unlicense; Apache-2.0
applies only to rrrriki's copyrightable human-authored contributions.

The legacy `k_shortest_path` and `library/graph/k_shortest_path.hpp` closely
match [ei1333/library's k-shortest-path implementation at the same revision](https://github.com/ei1333/library/blob/088b4c8b49f85194099305d998e7113dc3d5dd2e/graph/shortest-path/k-shortest-path.hpp),
also under the Unlicense. The historical source is unconfirmed. Local changes
handle nonpositive `k` and overflowing candidate distances.

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

### NyaanNyaan/library — UnionFindWithPotential

- User-provided reference: <https://nyaannyaan.github.io/library/data-structure/union-find-with-potential.hpp.html>
- Comparison source: <https://github.com/NyaanNyaan/library/blob/b3981adc80a800b2584980b01821324ea6c77183/data-structure/union-find-with-potential.hpp>
- License: CC0 1.0 Universal; [license at the reviewed commit](https://github.com/NyaanNyaan/library/blob/b3981adc80a800b2584980b01821324ea6c77183/LICENSE).

The implementation within `cpptemplate` matches the upstream algorithm. The
reviewed revision is a comparison point, not a claim of the exact historical
revision used. The independent header adds standard includes and namespace
qualification; the template retains its existing interface and comments.
The upstream material remains CC0; any copyrightable local additions are
subject only to their applicable rights, not a blanket relicensing of
`cpptemplate`.

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
  required by maintained headers including EulerTour. The `paizatemplate`
  snippet additionally contains a `dsu` whose implementation matches
  [ACL v1.5.1](https://github.com/atcoder/ac-library/blob/v1.5.1/atcoder/dsu.hpp)
  after removing the namespace and original reference comment; the exact
  historical acquisition path is unconfirmed. The comparison revision is
  available under [CC0-1.0](https://github.com/atcoder/ac-library/blob/v1.5.1/LICENSE).
  This embedded snippet is distinct from the external dependency used by headers.
- [jekyll-theme-minimal](https://github.com/pages-themes/minimal), CC0 1.0
  Universal: selected by the documentation build environment; it is not
  vendored in this repository.

## Legacy snippets with incomplete provenance

### Dynamic lazy segment tree

The user recalls reading [yamadanull's ABC435 E editorial](https://atcoder.jp/contests/abc435/editorial/14743) and believes they likely asked AI to generate an ACL-style implementation from scratch, rather than asking it to rewrite the editorial's implementation. The user is not certain of that memory. The editorial describes a dynamic lazy segment tree and links a submission; its author says that submission's tree was based on maspypy's library and uses a different argument order from ACL. The linked [submission](https://atcoder.jp/contests/abc435/submissions/71571477) returned HTTP 403 to the web tool, but the user supplied its code as text and confirmed that it is the editorial's example. Comparison shows similar recursive boundary-search structure and initialization of missing intervals, but different node ownership, public API and action argument ordering. The supplied example also has persistent-tree operations absent from the local tree. The current implementation uses ACL-style `mapping(f, x)` and `composition(f, g)` (f after g); the example uses `g(x, action)` and `h(old_action, new_action)`.

These similarities alone do not establish copying or adaptation: recursive
segment-tree techniques can arise independently. The user's account is a
qualified recollection, not proof of the precise prompt, inputs, or provenance
of each generated fragment. No license for the submission was established in
this review. The tree's specific provenance and reuse terms remain unresolved;
the user's account should not be represented as a finding that the example was
copied or rewritten.

The implementation was present in the initial repository snapshot
[`ae8ba1a`](https://github.com/5UG4M1202/competitive-library/blob/ae8ba1a/cpp.json),
which also does not establish those points. The user separately identified
betrue12's cheat sheet as a reference for segment-tree operation definitions.
AI involvement is uncertain and is not recorded as established authorship.

`library/data_structure/dynamic_lazy_segtree.hpp` and the corresponding
`cpptemplate` definition retain the existing algorithm and receive standard
includes, a fix for boundary searches with non-identity initial values,
automatic node ownership, move support, and disabled shallow copying.
Apache-2.0 applies to rrrriki's copyrightable human-authored contributions;
this is not a grant of rights held by others or a warranty about every fragment.

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

The remaining snippets have mixed or incomplete provenance and are not
covered by a collection-wide Apache-2.0 grant. Individual contributions are
covered only to the extent described in `LICENSE-SCOPE.md`. Evidence includes:

- The user reports using [betrue12's ACL lazy-segtree cheat sheet](https://betrue12.hateblo.jp/entry/2020/09/23/005940) as a reference for segment trees. The `@segtree_dynamic` range-assignment/range-sum definitions and the corrected `@segtree_lazy` range-add/range-sum example resemble corresponding examples. The dynamic example now uses an optional action rather than a numeric sentinel. This does not establish the provenance of the entire dynamic tree or the terms for copying the article's code; review those separately.
- The interval update and propagation in `dual_segtree` closely resemble [ACL lazy_segtree v1.6](https://github.com/atcoder/ac-library/blob/v1.6/atcoder/lazysegtree.hpp), whose [license is CC0-1.0](https://github.com/atcoder/ac-library/blob/v1.6/LICENSE). The actual historical source is not confirmed and the comparison alone does not establish copying. The separated header renames the global power-of-two helper to reduce name collisions. Apache-2.0 applies only to rrrriki's copyrightable human-authored contributions.
- The `rle` implementations in both submission templates and `library/string/run_length_encoding.hpp` have unconfirmed provenance. Empty-input handling was fixed in both templates and the separated header. Apache-2.0 applies only to rrrriki's copyrightable human-authored contributions.
- The `pairhash` snippet closely matches the `phash` code for `pair<int,int>` in [tqk's article](https://qiita.com/tqk/items/7c4df1eeb5ebaedcf714), except for using `long long` and formatting. Its historical source and applicable reuse terms are not established. The snippet's incomplete set declaration was changed to a named placeholder. Apache-2.0 applies only to rrrriki's copyrightable human-authored contributions; comparison alone does not establish the source or rights.
- The previous `out_div` in both templates matched the implementation in [DNEK's 2020 article](https://blog.dnek.net/entry/2020/12/06/011925) apart from types and formatting. Its actual historical source and applicable reuse terms are unconfirmed. The separated implementation was changed to use the remainder and avoid multiplication. The trial-division and other rounding helpers have unconfirmed provenance. Apache-2.0 applies only to rrrriki's copyrightable human-authored contributions; comparison alone does not establish the source or rights.
- The user confirms that the previous `next_combination` in `cpptemplate` was copied directly from [mdstoy's 2019 article](https://qiita.com/mdstoy/items/39104b686540c5f1dc6c); `paizatemplate` contained a matching implementation. That copied code is outside the repository's Apache-2.0 grant and may remain in history and past releases. [Qiita's terms, Article 7(3)](https://qiita.com/terms), provide for free use of posted code by other registered users; redistribution and modification rights for all downstream recipients were not established. The article says its code was reimplemented with reference to [a blog post](https://ysugiyama.hatenablog.com/entry/2018/10/31/233418), which includes code attributed to Thomas Draper and linked to Stack Overflow; this reference chain does not establish derivation. The current implementation in both templates was generated by AI in a separate chat from a behavioral specification. The generating chat reports not reading the prior implementation, the repository, external sources or earlier conversations, and not performing a web search. This is a reported process, not independently verified proof or a guarantee of the absence of third-party rights. Apache-2.0 applies only to the owner's copyrightable human contributions, within rights the owner can license.
- The user confirms creating `PrimeUtil` using generative AI. The maintained header and its template counterpart are offered under Apache-2.0 as described in `LICENSE-SCOPE.md`.
- `to_digit` is retained in the legacy template. Its historical provenance is unconfirmed. Apache-2.0 applies to rrrriki's copyrightable human-authored contributions.
- `_pow` and `mnht` are retained in both legacy templates. Their historical provenance is unconfirmed. Negative exponents are asserted against for `_pow`, and `mnht` makes standard `abs` available while retaining argument-dependent lookup. Apache-2.0 applies to rrrriki's copyrightable human-authored contributions.
- `library/misc/mo.hpp` reorganizes the offline range-processing approach in the legacy `@mos` snippet into a callback API. The snippet's right endpoint conversion and zero-query handling were corrected. The original snippet's historical provenance is unconfirmed; Apache-2.0 applies to rrrriki's copyrightable human-authored contributions.
- The `@meguru` snippet uses the widely described sentinel-based binary search approach; its historical source is unconfirmed. The endpoint termination check was changed to avoid signed subtraction overflow. A general explanation is [drken's article](https://qiita.com/drken/items/97e37dd6143e33a64c8c), but this comparison does not establish the snippet's origin or reuse terms.
- The template input/output helpers and macros have unconfirmed historical provenance. Forward declarations were added to support nested input, and failed `vector<bool>` input no longer reads an uninitialized value. Apache-2.0 applies to rrrriki's copyrightable human-authored contributions.
- `chmax` and `chmin` match examples in [AtCoder's APG4b tutorial](https://atcoder.jp/contests/apg4b/tasks/APG4b_ah), but their historical source and applicable terms are unconfirmed. `ordered_set_less` is a snippet alias using external GNU libstdc++ PBDS; the legacy alias's historical provenance is unconfirmed. Apache-2.0 applies to rrrriki's copyrightable human-authored contributions and does not relicense those external materials.
- `rotate_matrix`, `levenstein_distance`, `CoordCompressor`, `Doubling`, `GRID`, `BipartiteGraph`, `dijkstra`, and `kruskal` were separated from the legacy template. The comparison source for `Graph` is documented above; the exact provenance of the two algorithms has not been established by this review. `Doubling` retains the legacy reference to [an AtCoder submission](https://atcoder.jp/contests/abc377/submissions/71537117), whose relationship to the implementation was not established. Negative search results do not establish authorship or guarantee absence of third-party rights. Apache-2.0 applies only to the repository owner's copyrightable human-authored contributions. `BipartiteGraph` and `kruskal` depend on the external CC0-1.0 AtCoder Library.
- `CumulativeSum2D` in `cpptemplate` and `library/data_structure/cumulative_sum_2d.hpp` has a close structural match with [ei1333/library's implementation at `088b4c8`](https://github.com/ei1333/library/blob/088b4c8b49f85194099305d998e7113dc3d5dd2e/dp/cumulative-sum-2d.hpp), with axis order and comments changed. The [comparison revision's license is the Unlicense](https://github.com/ei1333/library/blob/088b4c8b49f85194099305d998e7113dc3d5dd2e/LICENSE). This is evidence for a possible adaptation, not proof of the historical source; upstream rights remain available under their terms, and Apache-2.0 applies only to rrrriki's copyrightable human-authored contributions. The exact provenance remains under review.
- The legacy `RollingHash` and `library/string/rolling_hash.hpp` closely match
  the [archived ei1333/library implementation](https://competitive-verifier.github.io/zzz_ei1333_library/string/rolling-hash.hpp.html).
  The [comparison repository uses the Unlicense](https://github.com/competitive-verifier/zzz_ei1333_library/blob/master/LICENSE).
  Differences include the random base range, member order, comments, and a local
  fix for signed `char` in string hashing. The historical source path remains
  unconfirmed; similarity alone does not establish it. Upstream rights remain
  available under their terms; Apache-2.0 applies only to rrrriki's copyrightable
  human-authored contributions. The retained Qiita link describes the technique.
- Several snippets contain AtCoder submission links or author handles, while
  other sections contain no source URL. A public submission or an unmarked
  section does not by itself establish authorship or relicensing authority.

These observations are not a complete file-level license map and do not
establish that unmarked sections are original or freely licensed.

Review and record each snippet separately before changing the license scope or
redistributing the snapshot as a licensed collection.
