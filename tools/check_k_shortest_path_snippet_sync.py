#!/usr/bin/env python3
"""Compare K-shortest-path in cpptemplate with its standalone header."""
from pathlib import Path
import os
import re
import shlex
import subprocess
import sys
import tempfile

sys.dont_write_bytecode = True
import check_interval_set_snippet_sync as shared
import check_graph_algorithms_snippet_sync as graph_shared

ROOT = Path(__file__).resolve().parents[1]
HEADER = 'library/graph/k_shortest_path.hpp'
MARKER = 'k_shortest_path(const Graph<T>'


def definition(source):
    if source.count(MARKER) != 1:
        raise ValueError('k_shortest_path definition must occur exactly once')
    pos = source.index(MARKER)
    start = source.rfind('template <typename T>', 0, pos)
    if start < 0:
        raise ValueError('template declaration missing')
    return source[start:source.index('\n}', pos) + 2]


def normalize(source):
    source = re.sub(r'/\*.*?\*/|//[^\n]*', '', source, flags=re.DOTALL)
    return re.sub(r'\s+', '', source.replace('std::', ''))


def main():
    entry = shared.parse_jsonc((ROOT / 'cpp.json').read_text(encoding='utf-8'))['default']
    if entry.get('prefix') != 'cpptemplate':
        raise ValueError('cpptemplate prefix changed')
    body = '\n'.join(entry['body'])
    if 'graph/shortest-path/k-shortest-path.hpp' not in body:
        raise ValueError('comparison source notice missing')
    snippet = definition(body)
    if normalize(snippet) != normalize(definition((ROOT / HEADER).read_text(encoding='utf-8'))):
        raise ValueError('k_shortest_path snippet and header differ')
    source = (ROOT / 'verify/local/k_shortest_path.test.cpp').read_text(encoding='utf-8')
    include = '#include "../../' + HEADER + '"'
    if source.count(include) != 1:
        raise ValueError('test include not found uniquely')
    preamble = '#include <bits/stdc++.h>\nusing namespace std;\n'
    with tempfile.TemporaryDirectory(prefix='k-shortest-path-snippet-') as directory:
        path = Path(directory) / 'test.cpp'
        binary = Path(directory) / 'test'
        path.write_text(source.replace(include, preamble + graph_shared.graph_definition(body) + '\n' + snippet), encoding='utf-8')
        subprocess.run(shlex.split(os.environ.get('CXX', 'g++')) +
                       ['-std=c++20', '-O2', str(path), '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True)
    print('k_shortest_path: snippet matches header and passes tests')


if __name__ == '__main__':
    main()
