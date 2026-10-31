---
name: cp-library-release-check
description: Run a read-only preflight for a competitive-library commit, tag, artifact, or Release, covering intended content, tests and docs, identity privacy, provenance and license uncertainty, hashes, and version consistency. Use before publication or when asked whether a release is ready. Never use this skill to commit, push, publish, delete, rewrite history, or change settings.
---

# CP Library Release Check

1. Establish the exact intended files, version, tag, artifact, and publication scope. Read `AGENTS.md`, `docs/maintenance.md`, `docs/commit-conventions.md`, and `docs/release-process.md`. Check the `cpptemplate` Release URL, the intended tag and `cpp.json` artifact digest, and successful Actions for the exact Release commit as specified there.
2. Inspect tracked, staged, modified, and untracked files separately. Never assume every untracked proposal belongs in the release.
3. Check relevant documentation and tests. For maintained snippet mappings, verify header/snippet consistency, notices, and intended entry-only changes; treat unresolved legacy entries separately rather than accepting an arbitrary whole-file hash change.
4. Inspect both author and committer identity for privacy-safe names and email addresses. Do not print credentials or unnecessary personal data.
5. Treat provenance, third-party attribution, and license uncertainty separately from correctness; do not certify them without evidence.
6. Check the proposed commit classification and coherent change unit against `docs/commit-conventions.md`, then check tag, version, source URL, intended commit, artifact contents, and hashes for consistency. Report blockers, readiness, evidence, checks actually run, and checks not run.

This workflow is read-only. Committing, pushing, creating a tag or Release, deleting or rewriting history, changing visibility/settings, and handling credentials each require separate explicit authorization.
