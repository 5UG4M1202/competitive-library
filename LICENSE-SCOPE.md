---
title: License scope
layout: page
---

# License scope

Copyright 2026 rrrriki

The unmodified Apache License 2.0 text in [`LICENSE`](LICENSE) applies to the
copyrightable original contributions and additions made by rrrriki in this
repository, except where a file-level notice or
[`THIRD_PARTY_NOTICES.md`](THIRD_PARTY_NOTICES.md) says otherwise.

This includes the split library, its documentation and tests, repository
maintenance documentation, local verification tools, CI configuration, and
repository-local Codex skills to the extent that those files are original
contributions by rrrriki.

## Mixed-provenance snippet collection

[`cpp.json`](cpp.json) is a current snippet collection with mixed and not yet
fully resolved provenance. It is **not** offered as a whole under the
repository's blanket Apache-2.0 grant.

The maintained `@interval_set` entry is synchronized from
`library/data_structure/interval_set.hpp`. Its adapted upstream portions remain
available under CC0 1.0 Universal; Apache-2.0 applies only to rrrriki's
copyrightable additions and modifications in that entry. The entry retains a
source and license notice describing that split.

The maintained `@RBST` entry is synchronized from
`library/data_structure/lazy_reversible_rbst.hpp`. Its NyaanNyaan/library core
remains available under CC0 1.0 Universal; Apache-2.0 applies only to rrrriki's
copyrightable additions and modifications in that entry. It also retains a
source and license notice describing that split.

The maintained `@EulerT` entry is synchronized from
`library/graph/euler_tour.hpp`. The user reports that @rrrrikiOW wrote it with
GitHub Copilot assistance, and the legacy snippet names no concrete external
source for the implementation. Apache-2.0 applies to rrrriki's copyrightable
human-authored contributions in this component. That provenance statement is
not a warranty that every generated fragment is copyrightable or free of
third-party claims. AtCoder Library remains an external CC0 1.0 Universal
dependency and is not included in this repository.

Every other snippet remains excluded from the blanket Apache-2.0 grant until
it receives its own provenance review. The absence of a notice does not
establish originality, public-domain status, or permission to redistribute it.

## Third-party material

Third-party material keeps its original license. In particular, portions of
`library/data_structure/interval_set.hpp` were adapted from drken1215's CC0
IntervalSet, and `library/data_structure/lazy_reversible_rbst.hpp` was adapted
from NyaanNyaan/library's CC0 RBST. The upstream CC0 rights remain available
independently; Apache-2.0 applies only to rrrriki's copyrightable additions and
modifications.

AtCoder Library is referenced as an external dependency by
`library/graph/euler_tour.hpp`; no ACL source is vendored here.

See [`THIRD_PARTY_NOTICES.md`](THIRD_PARTY_NOTICES.md) for sources, license
evidence, modifications, generated-documentation templates, dependencies, and
remaining provenance uncertainty. This scope statement does not add conditions
to any third-party license.
