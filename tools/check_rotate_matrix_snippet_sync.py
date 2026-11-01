#!/usr/bin/env python3
"""Compare the two rotation definitions inside cpptemplate with their header."""
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
HEADER = 'library/misc/rotate_matrix.hpp'
MARKER = 'template <typename T>'


def definitions(source):
    pos = source.find('rotate_matrix(')
    if pos < 0:
        raise ValueError('rotation missing')
    start = source.rfind(MARKER, 0, pos)
    second = source.find('rotate_matrix(', pos + 1)
    end = source.find('\n}', second) + 2
    if start < 0 or second < 0 or end < 2:
        raise ValueError('rotation definitions not found')
    return source[start:end]


def normalize(source):
    source = re.sub(r'/\*.*?\*/|//[^\n]*', '', source, flags=re.DOTALL)
    return re.sub(r'\s+', '', source.replace('std::', ''))


def main():
    entry = shared.parse_jsonc((ROOT / 'cpp.json').read_text(encoding='utf-8'))['default']
    if entry.get('prefix') != 'cpptemplate':
        raise ValueError('cpptemplate prefix changed')
    snippet = definitions('\n'.join(entry['body']))
    if normalize(snippet) != normalize(definitions((ROOT / HEADER).read_text(encoding='utf-8'))):
        raise ValueError('matrix rotation snippet and header differ')
    source = (ROOT / 'verify/local/rotate_matrix.test.cpp').read_text(encoding='utf-8')
    include = '#include "../../' + HEADER + '"'
    if source.count(include) != 1:
        raise ValueError('test include not found uniquely')
    preamble = '#include <string>\n#include <vector>\nusing namespace std;\n'
    with tempfile.TemporaryDirectory(prefix='rotate-matrix-snippet-') as directory:
        path = Path(directory) / 'test.cpp'
        binary = Path(directory) / 'test'
        path.write_text(source.replace(include, preamble + snippet), encoding='utf-8')
        subprocess.run(shlex.split(os.environ.get('CXX', 'g++')) +
                       ['-std=c++20', '-O2', str(path), '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True)
    print('rotate_matrix: snippet matches header and passes tests')


if __name__ == '__main__':
    main()
