---
name: cp-library-add
description: Add a new independent competitive-programming library component, algorithm, or data structure with its focused header, documentation, navigation, and verification. Use when the request is to introduce a reusable component that does not already exist. Do not use for extending an existing API, review-only work, verification-only work, releases, or the one-time extraction of cpp.json.
---

# CP Library Add

1. Locate the repository root and read `AGENTS.md`, the root and category READMEs, and `docs/maintenance.md` before editing.
2. Search the repository, standard facilities, and AtCoder Library first. If an existing facility already fits, reuse it instead of duplicating it. A new header should add a distinct capability, strengthen the interface, or provide meaningful independent maintenance value; keep simple contest idioms, input/output convenience functions, macros, and thin aliases in snippets by default. A PBDS wrapper is justified when it adds functionality or improves its interface.
3. Define the smallest stable public API. Record endpoint conventions, preconditions, invariants, sentinel and overflow restrictions, time complexity, and memory complexity.
4. Add one independently includable `library/<category>/<name>.hpp`, a sibling `documentation_of` Markdown file, and the minimum navigation updates.
5. Add boundary tests and, when useful, a deterministic comparison against a simple model. Add a judge-backed test only when the problem genuinely exercises the API.
6. Run the repository commands in `AGENTS.md`; report what ran and what remains unexercised.

Update `docs/library-guide.md`, category navigation, and the verifier site entry point when appropriate so users can find the new component by the operation they need. Document selection criteria, a minimal example, dependencies, preconditions, and limits; link existing alternatives when they overlap.

Treat discoverability as a general requirement for every component, not only algebraic or complex structures: identify the user's task, compare relevant alternatives, and make limitations, ownership, example inputs/outputs, and links to verification easy to find.

Do not create a new `cpp.json` entry unless the task requests one. If an established maintained mapping already exists, follow the repository synchronization policy; never modify unrelated entries or the external live snippet. Do not install tools or publish repository changes unless the user separately authorizes that exact action.
