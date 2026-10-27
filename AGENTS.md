# Repository maintenance rules

- `cpp.json` is a fixed legacy snapshot. Keep it byte-for-byte unchanged unless the user explicitly requests a snippet-file edit. Do not generate it from `library/` or write extracted headers back into it.
- The live VS Code snippet file is outside this repository. Do not modify it during library maintenance.
- Preserve existing public APIs and semantics when extracting code. Add missing standard includes and `std::` qualification, but document every behavior change separately.
- Before adding or relicensing code, read `LICENSE-SCOPE.md`, `THIRD_PARTY_NOTICES.md`, and `docs/license-and-contest-use.md`. Record the source and license of third-party material, retain required notices, and do not place the excluded `cpp.json` under the project's Apache-2.0 grant without a separate provenance review.
- Use `library/<category>/<name>.hpp` with sibling `<name>.md`. The Markdown must describe endpoint conventions, preconditions, sentinel assumptions, overflow limits, complexity, and known issues.
- Every code change must update its tests and documentation. Prefer a small boundary test plus a deterministic randomized comparison with a simple model.
- Add `PROBLEM` tests only for a genuinely matching supported judge problem. Do not use an unrelated problem merely to obtain a verification badge.
- Run from the repository root:
  - `powershell -ExecutionPolicy Bypass -File tools/check_legacy_snapshot.ps1`
  - `powershell -ExecutionPolicy Bypass -File tools/run_local_tests.ps1`
  - when `competitive-verifier` is already installed: `bash tools/run_verifier.sh`; it writes resolve, verify, and generated documentation outputs to a temporary directory by default.
- Do not install tools, push, open a PR, publish a Release, enable Pages, or change repository/account settings unless the user explicitly requests that action.
- Follow [`docs/commit-conventions.md`](docs/commit-conventions.md) when proposing or creating commits. The convention classifies messages and change units; it does not authorize staging, committing, pushing, or changing `cpp.json`.
- Treat `.github/workflows/verify.yml` as a reviewed draft until it has run successfully in GitHub Actions.
- Keep `LICENSE`, `LICENSE-SCOPE.md`, `NOTICE`, and `THIRD_PARTY_NOTICES.md` in generated documentation artifacts when templates or project code are redistributed.
- Use the matching repository-local skill in `.agents/skills/` when a request fits it; do not force all skills into a pipeline. The available routes are addition, extension, verification/verifier integration, read-only review, and read-only release preflight.
- `cp-library-release-check` never authorizes committing, pushing, publishing, deleting, rewriting history, or changing settings.
