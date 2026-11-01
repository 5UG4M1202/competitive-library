#!/usr/bin/env python3
"""Test the decimal digit counter in cpptemplate."""
from pathlib import Path
import os
import shlex
import subprocess
import sys
import tempfile

sys.dont_write_bytecode = True
import check_interval_set_snippet_sync as shared

ROOT = Path(__file__).resolve().parents[1]


def definition(source):
    marker = 'inline int to_digit('
    if source.count(marker) != 1:
        raise ValueError('to_digit definition must occur exactly once')
    start = source.index(marker)
    return source[start:source.index('\n}', start) + 2]


def main():
    entry = shared.parse_jsonc((ROOT / 'cpp.json').read_text(encoding='utf-8'))['default']
    if entry.get('prefix') != 'cpptemplate':
        raise ValueError('cpptemplate prefix changed')
    snippet = definition('\n'.join(entry['body']))
    source = (ROOT / 'tools/fixtures/to_digit.cpp').read_text(encoding='utf-8')
    include = '// @snippet-definition@'
    if source.count(include) != 1:
        raise ValueError('test include not found uniquely')
    with tempfile.TemporaryDirectory(prefix='to-digit-snippet-') as directory:
        path, binary = Path(directory) / 'test.cpp', Path(directory) / 'test'
        preamble = '#include <bits/stdc++.h>\nusing namespace std;\n'
        path.write_text(source.replace(include, preamble + snippet), encoding='utf-8')
        subprocess.run(shlex.split(os.environ.get('CXX', 'g++')) + ['-std=c++20', '-O2', str(path), '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True)
    print('to_digit: expanded snippet passes tests')


if __name__ == '__main__':
    main()
