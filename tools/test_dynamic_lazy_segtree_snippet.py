#!/usr/bin/env python3
"""Compare the dynamic lazy tree in cpptemplate with its header and test both."""
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
HEADER = 'library/data_structure/dynamic_lazy_segtree.hpp'
TEST = ROOT / 'verify/local/dynamic_lazy_segtree.test.cpp'
MARKER = 'struct dynamic_lazy_segtree {'


def definition(source):
    if source.count(MARKER) != 1:
        raise ValueError('dynamic lazy segtree definition must occur exactly once')
    pos = source.index(MARKER)
    start = source.rfind('template <class S,', 0, pos)
    if start < 0:
        raise ValueError('dynamic lazy segtree template declaration not found')
    end = source.index('\n};', pos) + len('\n};')
    return source[start:end]


def normalize(source):
    source = re.sub(r'/\*.*?\*/|//[^\n]*', '', source, flags=re.DOTALL)
    return re.sub(r'\s+', '', source)


def main():
    entry = shared.parse_jsonc((ROOT / 'cpp.json').read_text(encoding='utf-8'))['default']
    if entry.get('prefix') != 'cpptemplate':
        raise ValueError('cpptemplate prefix changed')
    body = '\n'.join(entry['body'])
    snippet = definition(body)
    if 'user recalls likely AI generation' not in body or 'Apache-2.0 applies to rrrriki' not in body:
        raise ValueError('dynamic lazy segtree provenance notice missing')
    if normalize(snippet) != normalize(definition((ROOT / HEADER).read_text(encoding='utf-8'))):
        raise ValueError('dynamic lazy segtree differs from its header')
    source = TEST.read_text(encoding='utf-8')
    include = '#include "../../' + HEADER + '"'
    if source.count(include) != 1:
        raise ValueError('test include not found uniquely')
    replacement = ('#include <algorithm>\n#include <cassert>\n#include <functional>\n'
                   '#include <memory>\n#include <utility>\n#include <vector>\n' + snippet)
    with tempfile.TemporaryDirectory(prefix='dynamic-lazy-test-') as directory:
        path = Path(directory) / 'test.cpp'
        binary = Path(directory) / 'test'
        path.write_text(source.replace(include, replacement), encoding='utf-8')
        subprocess.run(shlex.split(os.environ.get('CXX', 'g++')) +
                       ['-std=c++20', '-O2', '-Wall', '-Wextra', str(path), '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True)
    print('dynamic lazy segtree: snippet matches header and passes tests')


if __name__ == '__main__':
    main()
