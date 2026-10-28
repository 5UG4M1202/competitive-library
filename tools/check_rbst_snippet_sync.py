#!/usr/bin/env python3
"""Validate the maintained @RBST snippet against its source header."""

from __future__ import annotations

import json
import os
from pathlib import Path
import re
import shlex
import subprocess
import sys
import tempfile

sys.dont_write_bytecode = True
import check_interval_set_snippet_sync as shared


REPOSITORY_ROOT = Path(__file__).resolve().parents[1]
HEADER_PATH = REPOSITORY_ROOT / "library/data_structure/lazy_reversible_rbst.hpp"
SNIPPET_NAME = "@RBST"


def normalize_definition(source: str) -> str:
    source = re.sub(r"/\*.*?\*/", "", source, flags=re.DOTALL)
    source = re.sub(r"//[^\n]*", "", source)
    source = source.replace("std::", "")
    source = source.replace("static_cast<int>(v.size())", "(int)v.size()")
    return re.sub(r"\s+", "", source)


def load_snippet() -> tuple[dict[str, object], str]:
    raw = shared.SNIPPET_PATH.read_text(encoding="utf-8")
    snippets = shared.parse_jsonc(raw)
    if not isinstance(snippets, dict):
        raise ValueError("cpp.json must contain a top-level object")
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
        "NyaanNyaan/library",
        "b3981adc80a800b2584980b01821324ea6c77183",
        "CC0-1.0",
        "Copyright 2026 rrrriki",
        "Apache-2.0",
        "LICENSE-SCOPE.md",
        "THIRD_PARTY_NOTICES.md",
    )
    missing = [notice for notice in required_notices if notice not in source]
    if missing:
        raise ValueError(f"{SNIPPET_NAME}: missing provenance notices: {missing}")
    return snippets, source


def compare_maintained_definitions(snippet_source: str) -> None:
    header_source = HEADER_PATH.read_text(encoding="utf-8")
    markers = (
        "template <typename Node>",
        "template <typename T, typename E>\nstruct LazyReversibleRBSTNode",
        "template <typename T, typename E, T (*f)",
    )
    for marker in markers:
        header_definition = shared.extract_function(header_source, marker)
        snippet_definition = shared.extract_function(snippet_source, marker)
        if normalize_definition(header_definition) != normalize_definition(
            snippet_definition
        ):
            raise ValueError(f"{SNIPPET_NAME}: definition differs at {marker!r}")


def compile_and_run(snippet_source: str) -> None:
    test_source = f"""
#include <bits/stdc++.h>
using namespace std;

{snippet_source}

struct Summary {{
  long long sum = 0;
  int length = 0;
}};
struct Add {{
  long long value = 0;
  friend bool operator==(const Add&, const Add&) = default;
}};
Summary combine(Summary a, Summary b) {{
  return {{a.sum + b.sum, a.length + b.length}};
}}
Summary map_add(Summary s, Add a) {{
  s.sum += a.value * s.length;
  return s;
}}
Add compose_add(Add a, Add b) {{ return {{a.value + b.value}}; }}
Summary reverse_summary(Summary s) {{ return s; }}
using Tree = LazyReversibleRBST<Summary, Add, combine, map_add, compose_add,
                                reverse_summary>;
Summary value(long long x) {{ return {{x, 1}}; }}

int main() {{
  Tree tree;
  auto root = tree.make_tree();
  vector<long long> model;
  mt19937 random(20261028);
  for (int step = 0; step < 800; ++step) {{
    int size = static_cast<int>(model.size());
    int operation = static_cast<int>(random() % 4);
    if (size == 0) operation = 0;
    if (size >= 30 && operation == 0) operation = 1;
    if (operation == 0) {{
      int position = static_cast<int>(random() % (size + 1));
      long long inserted = static_cast<int>(random() % 101) - 50;
      tree.insert(root, position, value(inserted));
      model.insert(model.begin() + position, inserted);
    }} else if (operation == 1) {{
      int position = static_cast<int>(random() % size);
      tree.erase(root, position);
      model.erase(model.begin() + position);
    }} else {{
      int left = static_cast<int>(random() % (size + 1));
      int right = static_cast<int>(random() % (size + 1));
      if (left > right) swap(left, right);
      if (operation == 2 && left != right) {{
        long long delta = static_cast<int>(random() % 11) - 5;
        tree.apply(root, left, right, Add{{delta}});
        for (int i = left; i < right; ++i) model[i] += delta;
      }} else {{
        tree.reverse(root, left, right);
        reverse(model.begin() + left, model.begin() + right);
      }}
    }}

    size = static_cast<int>(model.size());
    if (tree.size(root) != size) return 1;
    for (int query = 0; query < 5; ++query) {{
      int left = static_cast<int>(random() % (size + 1));
      int right = static_cast<int>(random() % (size + 1));
      if (left > right) swap(left, right);
      long long expected = accumulate(model.begin() + left, model.begin() + right, 0LL);
      Summary actual = tree.fold(root, left, right);
      if (actual.sum != expected || actual.length != right - left) return 2;
    }}
  }}
  while (root != nullptr) tree.erase(root, 0);
  return 0;
}}
"""

    compiler = shlex.split(os.environ.get("CXX", "g++"))
    with tempfile.TemporaryDirectory(prefix="rbst-snippet-") as directory:
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
    compare_maintained_definitions(snippet_source)
    compile_and_run(snippet_source)
    print(
        f"cpp.json JSONC parsed ({len(snippets)} snippets); {SNIPPET_NAME} matches "
        "the maintained definitions and passed deterministic regression tests"
    )
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, json.JSONDecodeError, subprocess.CalledProcessError) as error:
        print(f"RBST snippet synchronization check failed: {error}", file=sys.stderr)
        raise SystemExit(1)
