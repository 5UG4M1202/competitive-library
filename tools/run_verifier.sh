#!/usr/bin/env bash
set -euo pipefail

repository_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
output_directory="${1:-}"

if ! command -v competitive-verifier >/dev/null 2>&1; then
  echo "competitive-verifier is not available in PATH." >&2
  exit 127
fi

if [[ -z "${ACL_INCLUDE_PATH:-}" || ! -d "$ACL_INCLUDE_PATH/atcoder" ]]; then
  echo "Set ACL_INCLUDE_PATH to the AtCoder Library root containing atcoder/." >&2
  exit 127
fi
export CPLUS_INCLUDE_PATH="$ACL_INCLUDE_PATH${CPLUS_INCLUDE_PATH:+:$CPLUS_INCLUDE_PATH}"

if [[ -z "$output_directory" ]]; then
  output_directory="$(mktemp -d)"
else
  mkdir -p "$output_directory"
  output_directory="$(cd "$output_directory" && pwd)"
fi

verify_files="$output_directory/verify-files.json"
verify_result="$output_directory/verify-result.json"
jekyll_directory="$output_directory/jekyll"
runtime_directory="$output_directory/runtime"

mkdir -p "$runtime_directory"
export COMPETITIVE_VERIFY_CONFIG_PATH="$runtime_directory"

cd "$repository_root"

python3 tools/check_interval_set_snippet_sync.py
python3 tools/check_rbst_snippet_sync.py
python3 tools/check_euler_tour_snippet_sync.py

competitive-verifier oj-resolve \
  --include library verify \
  --config .competitive-verifier/config.toml >"$verify_files"

COMPETITIVE_VERIFY_FILES_PATH="$verify_files" \
  competitive-verifier verify \
    --check-error \
    --output "$verify_result"

COMPETITIVE_VERIFY_FILES_PATH="$verify_files" \
  competitive-verifier docs "$verify_result" \
    --include library verify \
    --docs .competitive-verifier/docs \
    --destination "$jekyll_directory"

cp LICENSE LICENSE-SCOPE.md NOTICE THIRD_PARTY_NOTICES.md "$jekyll_directory/"

printf 'verify files: %s\n' "$verify_files"
printf 'verify result: %s\n' "$verify_result"
printf 'generated Jekyll source: %s\n' "$jekyll_directory"
