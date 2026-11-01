#!/usr/bin/env python3
"""Compare the doubling implementation in cpptemplate with its header."""
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
HEADER = 'library/graph/doubling.hpp'
MARKER = 'struct Doubling {'


def definition(source):
    if source.count(MARKER) != 1:
        raise ValueError('Doubling definition must occur exactly once')
    pos = source.index(MARKER)
    start = source.rfind('template <typename T>', 0, pos)
    if start < 0:
        raise ValueError('template declaration missing')
    return source[start:source.index('\n};', pos) + len('\n};')]


def normalize(source):
    source = re.sub(r'/\*.*?\*/|//[^\n]*', '', source, flags=re.DOTALL)
    source = re.sub(r'\bll\b', 'long long', source.replace('std::', ''))
    return re.sub(r'\s+', '', source)


def main():
    entry = shared.parse_jsonc((ROOT / 'cpp.json').read_text(encoding='utf-8'))['default']
    if entry.get('prefix') != 'cpptemplate':
        raise ValueError('cpptemplate prefix changed')
    snippet = definition('\n'.join(entry['body']))
    if normalize(snippet) != normalize(definition((ROOT / HEADER).read_text(encoding='utf-8'))):
        raise ValueError('Doubling snippet and header differ')
    source = (ROOT / 'verify/local/doubling.test.cpp').read_text(encoding='utf-8')
    include = '#include "../../' + HEADER + '"'
    if source.count(include) != 1:
        raise ValueError('test include not found uniquely')
    preamble = '#include <cassert>\n#include <vector>\nusing namespace std;\nusing ll = long long;\n'
    with tempfile.TemporaryDirectory(prefix='doubling-snippet-') as directory:
        path = Path(directory) / 'test.cpp'
        binary = Path(directory) / 'test'
        path.write_text(source.replace(include, preamble + snippet), encoding='utf-8')
        subprocess.run(shlex.split(os.environ.get('CXX', 'g++')) +
                       ['-std=c++20', '-O2', str(path), '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True)
    print('Doubling: snippet matches header and passes tests')


if __name__ == '__main__':
    main()
