#!/usr/bin/env python3
"""Check and exercise the dual segment tree inside cpptemplate."""
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
HEADER = 'library/data_structure/dual_segtree.hpp'


def definition(source, helper):
    marker = 'struct dual_segtree {'
    if source.count(marker) != 1 or source.count('inline int ' + helper + '(') != 1:
        raise ValueError('dual segment tree definition is not unique')
    start = source.index('inline int ' + helper + '(')
    return source[start:source.index('\n};', source.index(marker)) + 3]


def normalize(source):
    source = re.sub(r'/\*.*?\*/|//[^\n]*', '', source, flags=re.DOTALL)
    return re.sub(r'\s+', '', source.replace('dual_segtree_ceil_pow2', 'ceil_pow2'))


def main():
    entry = shared.parse_jsonc((ROOT / 'cpp.json').read_text(encoding='utf-8'))['default']
    if entry.get('prefix') != 'cpptemplate':
        raise ValueError('cpptemplate prefix changed')
    snippet = definition('\n'.join(entry['body']), 'ceil_pow2')
    header = definition((ROOT / HEADER).read_text(encoding='utf-8'), 'dual_segtree_ceil_pow2')
    if normalize(snippet) != normalize(header):
        raise ValueError('dual segment tree snippet and header differ')
    source = (ROOT / 'verify/local/dual_segtree.test.cpp').read_text(encoding='utf-8')
    include = '#include "../../' + HEADER + '"'
    if source.count(include) != 1:
        raise ValueError('test include not found uniquely')
    with tempfile.TemporaryDirectory(prefix='dual-segtree-snippet-') as directory:
        path, binary = Path(directory) / 'test.cpp', Path(directory) / 'test'
        path.write_text(source.replace(include, '#include <bits/stdc++.h>\nusing namespace std;\n' + snippet), encoding='utf-8')
        subprocess.run(shlex.split(os.environ.get('CXX', 'g++')) +
                       ['-std=c++20', '-O2', str(path), '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True)
    print('dual_segtree: snippet matches header and passes tests')


if __name__ == '__main__':
    main()
