#!/usr/bin/env python3
"""Compare cpptemplate's bipartite graph wrapper with its header."""
from pathlib import Path
import os
import re
import shlex
import subprocess
import sys
import tempfile

sys.dont_write_bytecode = True
import check_interval_set_snippet_sync as shared

ROOT = Path(__file__).resolve().parents[1]
HEADER = 'library/graph/bipartite_graph.hpp'
MARKER = 'struct BipartiteGraph : '


def definition(source):
    if source.count(MARKER) != 1:
        raise ValueError('BipartiteGraph definition must occur exactly once')
    start = source.index(MARKER)
    return source[start:source.index('\n};', start) + len('\n};')]


def normalize(source):
    source = re.sub(r'/\*.*?\*/|//[^\n]*', '', source, flags=re.DOTALL)
    source = source.replace('std::', '').replace('atcoder::', '')
    return re.sub(r'\s+', '', source)


def main():
    entry = shared.parse_jsonc((ROOT / 'cpp.json').read_text(encoding='utf-8'))['default']
    if entry.get('prefix') != 'cpptemplate':
        raise ValueError('cpptemplate prefix changed')
    snippet = definition('\n'.join(entry['body']))
    if normalize(snippet) != normalize(definition((ROOT / HEADER).read_text(encoding='utf-8'))):
        raise ValueError('BipartiteGraph snippet and header differ')
    source = (ROOT / 'verify/local/bipartite_graph.test.cpp').read_text(encoding='utf-8')
    include = '#include "../../' + HEADER + '"'
    if source.count(include) != 1:
        raise ValueError('test include not found uniquely')
    preamble = '#include <atcoder/dsu>\n#include <vector>\nusing namespace std;\nusing namespace atcoder;\n'
    with tempfile.TemporaryDirectory(prefix='bipartite-graph-snippet-') as directory:
        path = Path(directory) / 'test.cpp'
        binary = Path(directory) / 'test'
        path.write_text(source.replace(include, preamble + snippet), encoding='utf-8')
        subprocess.run(shlex.split(os.environ.get('CXX', 'g++')) +
                       ['-std=c++20', '-O2', str(path), '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True)
    print('BipartiteGraph: snippet matches header and passes tests')


if __name__ == '__main__':
    main()
