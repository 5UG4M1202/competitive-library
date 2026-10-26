---
name: cp-library-add
description: Add a new independent competitive-programming library component, algorithm, or data structure with its focused header, documentation, navigation, and verification. Use when the request is to introduce a reusable component that does not already exist. Do not use for extending an existing API, review-only work, verification-only work, releases, or the one-time extraction of cpp.json.
---

# CP Library Add

1. Locate the repository root and read `AGENTS.md`, the root and category READMEs, and `docs/maintenance.md` before editing.
2. Search the repository and AtCoder Library first. If ACL or an existing component already fits the intended use, recommend or reuse it instead of duplicating it.
3. Define the smallest stable public API. Record endpoint conventions, preconditions, invariants, sentinel and overflow restrictions, time complexity, and memory complexity.
4. Add one independently includable `library/<category>/<name>.hpp`, a sibling `documentation_of` Markdown file, and the minimum navigation updates.
5. Add boundary tests and, when useful, a deterministic comparison against a simple model. Add a judge-backed test only when the problem genuinely exercises the API.
6. Run the repository commands in `AGENTS.md`; report what ran and what remains unexercised.

Do not modify `cpp.json` or the live VS Code snippet. Do not install tools or publish repository changes unless the user separately authorizes that exact action.
