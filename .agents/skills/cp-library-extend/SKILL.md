---
name: cp-library-extend
description: Extend the API or capability of an existing competitive-programming library component while preserving compatibility, invariants, complexity, documentation, and regression coverage. Use for new operations or supported cases on an existing header. Do not use for an unrelated new component, review-only work, verification-only work, or release work.
---

# CP Library Extend

1. Read `AGENTS.md`, `docs/maintenance.md`, and the target header, sibling documentation, callers, and tests.
2. Characterize the current API and observable semantics before editing. Identify compatibility risks, breaking changes, invariant changes, and complexity changes.
3. Prefer the smallest extension that matches the existing conventions; avoid speculative abstractions.
4. Preserve known quirks unless the user authorizes a bug fix. For an authorized fix, keep a minimal failing regression and distinguish the prior behavior from the intended result.
5. Update the sibling documentation and add focused boundary and regression coverage with the code change.
6. If the component has a mapped maintained entry in `cpp.json`, synchronize only that entry from the header, retain its provenance/license notices and unrelated snippet metadata, then run the mapped snippet check.
7. Run the repository commands in `AGENTS.md`; report compatibility decisions, results, and remaining gaps.

Do not blanket-regenerate unresolved entries or modify the live VS Code snippet. Do not publish or change repository settings without separate explicit authorization.
