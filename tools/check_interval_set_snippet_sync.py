#!/usr/bin/env python3
"""Validate the maintained IntervalSet snippet against its source header."""

from __future__ import annotations

import json
import os
from pathlib import Path
import re
import shlex
import subprocess
import sys
import tempfile


REPOSITORY_ROOT = Path(__file__).resolve().parents[1]
SNIPPET_PATH = REPOSITORY_ROOT / "cpp.json"
HEADER_PATH = REPOSITORY_ROOT / "library/data_structure/interval_set.hpp"
SNIPPET_NAME = "@interval_set"


def extract_function(source: str, marker: str) -> str:
    start = source.find(marker)
    if start < 0:
        raise ValueError(f"function marker not found: {marker}")
    opening = source.find("{", start)
    if opening < 0:
        raise ValueError(f"opening brace not found after: {marker}")

    depth = 0
    for index in range(opening, len(source)):
        if source[index] == "{":
            depth += 1
        elif source[index] == "}":
            depth -= 1
            if depth == 0:
                return source[start : index + 1]
    raise ValueError(f"closing brace not found after: {marker}")


def normalize_function(source: str) -> str:
    return re.sub(r"\s+", "", source.replace("std::", ""))


def parse_jsonc(source: str) -> object:
    """Remove JSONC comments and trailing commas without touching strings."""

    without_comments: list[str] = []
    index = 0
    in_string = False
    escaped = False
    while index < len(source):
        character = source[index]
        if in_string:
            without_comments.append(character)
            if escaped:
                escaped = False
            elif character == "\\":
                escaped = True
            elif character == '"':
                in_string = False
            index += 1
            continue

        if character == '"':
            in_string = True
            without_comments.append(character)
            index += 1
        elif source.startswith("//", index):
            index += 2
            while index < len(source) and source[index] not in "\r\n":
                index += 1
        elif source.startswith("/*", index):
            index += 2
            while index < len(source) and not source.startswith("*/", index):
                if source[index] in "\r\n":
                    without_comments.append(source[index])
                index += 1
            if index >= len(source):
                raise ValueError("unterminated block comment in cpp.json")
            index += 2
        else:
            without_comments.append(character)
            index += 1

    uncommented = "".join(without_comments)
    without_trailing_commas: list[str] = []
    index = 0
    in_string = False
    escaped = False
    while index < len(uncommented):
        character = uncommented[index]
        if in_string:
            without_trailing_commas.append(character)
            if escaped:
                escaped = False
            elif character == "\\":
                escaped = True
            elif character == '"':
                in_string = False
            index += 1
            continue

        if character == '"':
            in_string = True
            without_trailing_commas.append(character)
        elif character == ",":
            lookahead = index + 1
            while lookahead < len(uncommented) and uncommented[lookahead].isspace():
                lookahead += 1
            if lookahead < len(uncommented) and uncommented[lookahead] in "]}":
                index += 1
                continue
            without_trailing_commas.append(character)
        else:
            without_trailing_commas.append(character)
        index += 1

    return json.loads("".join(without_trailing_commas))


def load_snippet() -> tuple[dict[str, object], str]:
    raw = SNIPPET_PATH.read_text(encoding="utf-8")
    snippets = parse_jsonc(raw)
    if not isinstance(snippets, dict):
        raise ValueError("cpp.json must contain a top-level object")

    for name, entry in snippets.items():
        if not isinstance(entry, dict):
            raise ValueError(f"{name}: snippet entry must be an object")
        prefix = entry.get("prefix")
        if not isinstance(prefix, str) and not (
            isinstance(prefix, list) and all(isinstance(item, str) for item in prefix)
        ):
            raise ValueError(f"{name}: prefix must be a string or string array")
        body = entry.get("body")
        if not isinstance(body, str) and not (
            isinstance(body, list) and all(isinstance(item, str) for item in body)
        ):
            raise ValueError(f"{name}: body must be a string or string array")
        if "description" in entry and not isinstance(entry["description"], str):
            raise ValueError(f"{name}: description must be a string")

    entry = snippets.get(SNIPPET_NAME)
    if not isinstance(entry, dict):
        raise ValueError(f"missing snippet: {SNIPPET_NAME}")
    if entry.get("prefix") != SNIPPET_NAME:
        raise ValueError(f"{SNIPPET_NAME}: prefix changed")
    body = entry.get("body")
    if not isinstance(body, list) or not all(isinstance(line, str) for line in body):
        raise ValueError(f"{SNIPPET_NAME}: body must be an array of strings")

    source = "\n".join(body)
    required_notices = (
        "drken1215/algorithm",
        "CC0-1.0",
        "Copyright 2026 rrrriki",
        "Apache License 2.0",
        "https://www.apache.org/licenses/LICENSE-2.0",
        "LICENSE-SCOPE.md",
        "THIRD_PARTY_NOTICES.md",
    )
    missing = [notice for notice in required_notices if notice not in source]
    if missing:
        raise ValueError(f"{SNIPPET_NAME}: missing provenance notices: {missing}")

    return snippets, source


def normalize_interval_set(source: str) -> str:
    """Normalize only the known spelling differences in the mapped definition."""
    literals = []

    def protect_literals(match: re.Match[str]) -> str:
        token = match.group()
        if token.startswith(("//", "/*")):
            return " "
        literals.append(token)
        return f"__INTERVAL_SET_LITERAL_{len(literals) - 1}__"

    source = re.sub(r'''"(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*'|/\*.*?\*/|//[^\n]*''',
                    protect_literals, source, flags=re.DOTALL)
    source = source.replace("std::", "")
    source = source.replace("class VAL = ll", "class VAL = long long")
    source = source.replace("Node(const T& l_, const T& r_, const VAL& val_)",
                            "Node(const T& l, const T& r, const VAL& val)")
    source = source.replace(": l(l_), r(r_), val(val_)", ": l(l), r(r), val(val)")
    source = source.replace("const VAL& identity_ = VAL()", "const VAL& identity = VAL()")
    source = source.replace(": identity(identity_)", ": identity(identity)")
    source = source.replace("static_cast<int>(v.size())", "(int)v.size()")
    source = source.replace("static_cast<T>(l)", "(T)l")
    source = source.replace("static_cast<T>(r)", "(T)r")
    source = source.replace("static_cast<T>(r - l)", "(T)(r - l)")
    source = source.replace("Node(l, T{},", "Node(l, 0,")
    source = source.replace("return (it != S.end() && r <= it->r);",
                            "return it != S.end() && r <= it->r;")
    source = source.replace("total_cover += (R - L)", "total_cover += R - L")
    source = source.replace("total_cover -= (R - L)", "total_cover -= R - L")
    source = source.replace("for (const auto& e : ins.S)", "for (auto& e : ins.S)")
    source = re.sub(r"\s+", "", source)
    for index, literal in enumerate(literals):
        source = source.replace(f"__INTERVAL_SET_LITERAL_{index}__", literal)
    return source


def compare_maintained_function(snippet_source: str) -> None:
    header_source = HEADER_PATH.read_text(encoding="utf-8")
    marker = "template <class T, class VAL"
    header_definition = extract_function(header_source, marker)
    snippet_definition = extract_function(snippet_source, marker)
    if normalize_interval_set(header_definition) != normalize_interval_set(snippet_definition):
        raise ValueError("@interval_set definition differs from the maintained header")


def compile_and_run(snippet_source: str) -> None:
    test_source = f"""
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

{snippet_source}

int main() {{
  {{
    IntervalSet<long long, int> intervals(0);
    intervals.update(2, 3, 1);
    intervals.update(3, 6, 2);
    intervals.update(6, 8, 3);
    if (intervals.contained_intervals_in(2, 8) != 3) return 1;
    if (intervals.contained_intervals_in(3, 8) != 2) return 2;
    if (intervals.contained_intervals_in(2, 6) != 2) return 3;
  }}

  constexpr int domain = 32;
  vector<int> model(domain, 0);
  IntervalSet<int, int> intervals(0);
  mt19937 random(20261027);

  auto expected_count = [&](int left, int right) {{
    long long count = 0;
    for (int index = 0; index < domain;) {{
      if (model[index] == 0) {{
        ++index;
        continue;
      }}
      int end = index + 1;
      while (end < domain && model[end] == model[index]) ++end;
      if (left <= index && end <= right) ++count;
      index = end;
    }}
    return count;
  }};

  auto check = [&](int left, int right) {{
    const auto expected = expected_count(left, right);
    const auto actual = intervals.contained_intervals_in(left, right);
    if (actual != expected) {{
      cerr << "query [" << left << ", " << right << "): expected "
           << expected << ", actual " << actual << '\\n';
      return false;
    }}
    return true;
  }};

  for (int step = 0; step < 600; ++step) {{
    int left = static_cast<int>(random() % (domain + 1));
    int right = static_cast<int>(random() % (domain + 1));
    if (left > right) swap(left, right);
    if (random() % 4 == 0) {{
      intervals.erase(left, right);
      fill(model.begin() + left, model.begin() + right, 0);
    }} else {{
      const int value = 1 + static_cast<int>(random() % 4);
      intervals.update(left, right, value);
      fill(model.begin() + left, model.begin() + right, value);
    }}

    for (int query = 0; query < 8; ++query) {{
      int query_left = static_cast<int>(random() % (domain + 1));
      int query_right = static_cast<int>(random() % (domain + 1));
      if (query_left > query_right) swap(query_left, query_right);
      if (!check(query_left, query_right)) return 10;
    }}
    if (step % 97 == 0) {{
      for (int query_left = 0; query_left <= domain; ++query_left) {{
        for (int query_right = query_left; query_right <= domain; ++query_right) {{
          if (!check(query_left, query_right)) return 11;
        }}
      }}
    }}
  }}
  return 0;
}}
"""

    compiler = shlex.split(os.environ.get("CXX", "g++"))
    with tempfile.TemporaryDirectory(prefix="interval-set-snippet-") as directory:
        directory_path = Path(directory)
        source_path = directory_path / "snippet_test.cpp"
        executable_path = directory_path / (
            "snippet_test.exe" if os.name == "nt" else "snippet_test"
        )
        source_path.write_text(test_source, encoding="utf-8")
        subprocess.run(
            compiler
            + [
                "-std=c++20",
                "-O2",
                "-Wall",
                "-Wextra",
                "-Wshadow",
                "-Wconversion",
                "-pedantic",
                str(source_path),
                "-o",
                str(executable_path),
            ],
            check=True,
        )
        subprocess.run([str(executable_path)], check=True)


def main() -> int:
    snippets, snippet_source = load_snippet()
    compare_maintained_function(snippet_source)
    compile_and_run(snippet_source)
    print(
        f"cpp.json JSONC parsed ({len(snippets)} snippets); {SNIPPET_NAME} matches the "
        "maintained definition and passed deterministic regression tests"
    )
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, json.JSONDecodeError, subprocess.CalledProcessError) as error:
        print(f"snippet synchronization check failed: {error}", file=sys.stderr)
        raise SystemExit(1)
