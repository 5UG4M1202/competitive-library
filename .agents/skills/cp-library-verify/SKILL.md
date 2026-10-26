---
name: cp-library-verify
description: Add, repair, or assess competitive-programming library tests and competitive-verifier integration, including selecting a genuinely suitable judge problem. Use for local verification, randomized or boundary tests, verifier configuration, test metadata, or CI verification. Do not use to add features, silently fix production code, or publish results.
---

# CP Library Verify

1. Read `AGENTS.md`, `verify/README.md`, `docs/maintenance.md`, the verifier config and workflow, and the target header, documentation, and tests.
2. Label each test as intended-correctness, characterization, or a known failing regression. Keep minimal counterexamples and expected mathematical results; never weaken expectations to make a suite green.
3. Run the available compiler and deterministic local tests. Do not change production code unless the user separately authorizes a fix.
4. Use `PROBLEM` only for a supported problem that actually models the target API. Otherwise use `STANDALONE` or `LOCALCASE` and state why no judge-backed test was added.
5. Before changing verifier integration, confirm the current official `STANDALONE`, `LOCALCASE`, `PROBLEM`, and `documentation_of` interfaces. Keep GitHub Actions configured so test failures fail the job; do not rely on a default that skips checks.
6. Report separately what was executed, what was only drafted, and what was not tested.

Do not install verification tools, enable Pages, change settings, modify `cpp.json`, or publish without separate explicit authorization.
