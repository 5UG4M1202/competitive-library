#!/usr/bin/env python3
"""Validate the maintained @EulerT snippet against its source header."""

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
HEADER_PATH = REPOSITORY_ROOT / "library/graph/euler_tour.hpp"
SNIPPET_NAME = "@EulerT"


def normalize_definition(source: str) -> str:
    source = re.sub(r"/\*.*?\*/", "", source, flags=re.DOTALL)
    source = re.sub(r"//[^\n]*", "", source)
    source = source.replace("std::", "").replace("atcoder::", "")
    source = source.replace("static_cast<int>(e.to)", "(int)e.to")
    source = re.sub(r"\blong long\b", "ll", source)
    source = source.replace("10000000000LL", "(ll)1e10")
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
        "@rrrrikiOW",
        "GitHub Copilot",
        "Copyright 2026 rrrriki",
        "Apache-2.0",
        "AtCoder Library",
        "CC0-1.0",
        "LICENSE-SCOPE.md",
        "THIRD_PARTY_NOTICES.md",
    )
    missing = [notice for notice in required_notices if notice not in source]
    if missing:
        raise ValueError(f"{SNIPPET_NAME}: missing provenance notices: {missing}")
    return snippets, source


def compare_maintained_definition(snippet_source: str) -> None:
    header_source = HEADER_PATH.read_text(encoding="utf-8")
    marker = "template <typename E_T, typename V_T>\nstruct EulerTour"
    header_definition = shared.extract_function(header_source, marker)
    snippet_definition = shared.extract_function(snippet_source, marker)
    normalized_header = normalize_definition(header_definition)
    normalized_snippet = normalize_definition(snippet_definition)
    if normalized_header != normalized_snippet:
        mismatch = next(
            (
                index
                for index, (left, right) in enumerate(
                    zip(normalized_header, normalized_snippet)
                )
                if left != right
            ),
            min(len(normalized_header), len(normalized_snippet)),
        )
        context_start = max(0, mismatch - 40)
        context_end = mismatch + 80
        raise ValueError(
            f"{SNIPPET_NAME}: EulerTour differs from maintained header near "
            f"normalized offset {mismatch}: header="
            f"{normalized_header[context_start:context_end]!r}, snippet="
            f"{normalized_snippet[context_start:context_end]!r}"
        )


def compile_and_run(snippet_source: str) -> None:
    test_source = f"""
#include <bits/stdc++.h>
#include <atcoder/lazysegtree>
#include <atcoder/segtree>
using namespace std;
using namespace atcoder;
using ll = long long;

template <typename T = int>
struct Graph {{
  struct Edge {{ int to; T cost; }};
  vector<vector<Edge>> g;
  explicit Graph(int n) : g(n) {{}}
  size_t size() const {{ return g.size(); }}
  vector<Edge>& operator[](int v) {{ return g[v]; }}
  const vector<Edge>& operator[](int v) const {{ return g[v]; }}
  void add_edge(int u, int v, T cost) {{
    g[u].push_back({{v, cost}});
    g[v].push_back({{u, cost}});
  }}
}};

{snippet_source}

int main() {{
  Graph<ll> graph(5);
  graph.add_edge(0, 1, 2);
  graph.add_edge(0, 2, 3);
  graph.add_edge(1, 3, 5);
  graph.add_edge(1, 4, 7);
  vector<ll> vertex = {{10, 20, 30, 40, 50}};
  EulerTour<ll, ll> tour(graph, 0, vertex);
  if (tour.lca(3, 4) != 1 || tour.lca(3, 2) != 0) return 1;
  if (tour.query_edge(3, 4) != 12 || tour.query_edge(3, 2) != 10) return 2;
  if (tour.query_vert(3, 4) != 90 || tour.query_vert(3, 3) != 0) return 3;
  if (tour.query_subtree_edge(1) != 12) return 4;
  if (tour.query_subtree_vert(1) != 110) return 5;
  tour.update_edge(1, 3, 11, 2);
  tour.add_cost_edge(1, 4, 3, 2);
  if (tour.query_edge(3, 4) != 21 || tour.query_subtree_edge(1) != 21)
    return 6;
  tour.update_vert(3, 44, 2);
  tour.add_cost_vert(4, 2, 2);
  if (tour.query_vert(3, 4) != 96 || tour.query_subtree_vert(1) != 116)
    return 7;
  tour.add_cost_subtree_vert(1, 10, 2);
  tour.add_cost_subtree_edge(1, 2, 2);
  if (tour.query_vert(3, 4) != 116 || tour.query_subtree_vert(1) != 146)
    return 8;
  if (tour.query_edge(3, 4) != 25 || tour.query_subtree_edge(1) != 25)
    return 9;
  tour.update_vert(0, 100, 2);
  if (tour.get_vert(0, 0) != 100 || tour.get_vert(0, 1) != 100)
    return 10;
  if (tour.query_vert(3, 2) != 114 || tour.query_subtree_vert(0) != 276)
    return 11;

  Graph<ll> singleton_graph(1);
  EulerTour<ll, ll> singleton(singleton_graph, 0, vector<ll>{{7}});
  singleton.update_vert(0, 11, 2);
  singleton.add_cost_vert(0, 4, 2);
  if (singleton.get_vert(0, 0) != 15 || singleton.get_vert(0, 1) != 15)
    return 12;
  if (singleton.query_subtree_vert(0) != 15 || singleton.query_vert(0, 0) != 0)
    return 13;
  return 0;
}}
"""

    compiler = shlex.split(os.environ.get("CXX", "g++"))
    include_args: list[str] = []
    acl_include = os.environ.get("ACL_INCLUDE_PATH")
    if acl_include:
        include_args = ["-I", acl_include]
    with tempfile.TemporaryDirectory(prefix="euler-tour-snippet-") as directory:
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
            ]
            + include_args
            + [str(source_path), "-o", str(executable_path)],
            check=True,
        )
        subprocess.run([str(executable_path)], check=True)


def main() -> int:
    snippets, snippet_source = load_snippet()
    compare_maintained_definition(snippet_source)
    compile_and_run(snippet_source)
    print(
        f"cpp.json JSONC parsed ({len(snippets)} snippets); {SNIPPET_NAME} matches "
        "the maintained definition and passed the representative template test"
    )
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, json.JSONDecodeError, subprocess.CalledProcessError) as error:
        print(f"EulerTour snippet synchronization check failed: {error}", file=sys.stderr)
        raise SystemExit(1)
