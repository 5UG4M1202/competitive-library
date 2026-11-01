#!/usr/bin/env python3
"""Check RollingHash against cpptemplate and compile its expanded definition."""
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
HEADER = 'library/string/rolling_hash.hpp'


def definition(source):
    marker = 'struct RollingHash {'
    if source.count(marker) != 1:
        raise ValueError('RollingHash must occur exactly once')
    start = source.index(marker)
    return source[start:source.index('\n};', start) + 3]


def normalize(source):
    source = re.sub(r'/\*.*?\*/|//[^\n]*', '', source, flags=re.DOTALL)
    return re.sub(r'\s+', '', source.replace('std::', ''))


def main():
    entry = shared.parse_jsonc((ROOT / 'cpp.json').read_text(encoding='utf-8'))['default']
    if entry.get('prefix') != 'cpptemplate':
        raise ValueError('cpptemplate prefix changed')
    body = '\n'.join(entry['body'])
    if 'zzz_ei1333_library/string/rolling-hash.hpp.html' not in body:
        raise ValueError('comparison source notice missing')
    snippet = definition(body)
    if normalize(snippet) != normalize(definition((ROOT / HEADER).read_text(encoding='utf-8'))):
        raise ValueError('RollingHash snippet and header differ')
    source = (ROOT / 'verify/local/rolling_hash.test.cpp').read_text(encoding='utf-8')
    include = '#include "../../' + HEADER + '"'
    if source.count(include) != 1:
        raise ValueError('test include not found uniquely')
    with tempfile.TemporaryDirectory(prefix='rolling-hash-snippet-') as directory:
        path, binary = Path(directory) / 'test.cpp', Path(directory) / 'test'
        path.write_text(source.replace(include, '#include <bits/stdc++.h>\nusing namespace std;\n' + snippet), encoding='utf-8')
        subprocess.run(shlex.split(os.environ.get('CXX', 'g++')) +
                       ['-std=c++20', '-O2', str(path), '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True)
    print('RollingHash: snippet matches header and passes tests')


if __name__ == '__main__':
    main()
