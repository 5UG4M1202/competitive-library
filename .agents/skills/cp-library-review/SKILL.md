---
name: cp-library-review
description: Review or audit competitive-programming library code for correctness bugs, endpoint mistakes, invariant violations, unsafe assumptions, API inconsistencies, and complexity regressions. Use when the user asks for a bug check, audit, or review. This is read-only by default and does not authorize fixes, feature work, or publication.
---

# CP Library Review

1. Read `AGENTS.md`, the implementation, sibling documentation, tests, dependencies, and the requested diff or history scope.
2. Inspect contracts, endpoints, sentinels, overflow, iterator validity, complexity, and mismatches between docs, tests, and implementation.
3. Run available focused tests and use small deterministic cases or model comparisons where they improve confidence.
4. Report findings in severity order with exact locations, a minimal reproducer or counterexample, expected versus actual behavior, impact, and confidence.
5. Separate confirmed defects from concerns, contract ambiguities, and missing coverage. Check that characterization tests are not presented as proof of intended correctness.
6. If no defect is confirmed, say so and list residual risks or untested areas.

Do not modify code, tests, `cpp.json`, history, or external state unless the user separately requests a specific fix or action.
