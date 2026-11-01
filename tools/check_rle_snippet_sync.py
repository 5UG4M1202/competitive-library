#!/usr/bin/env python3
"""Compare and execute RLE definitions from both submission templates."""
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
HEADER = 'library/string/run_length_encoding.hpp'


def definitions(source):
    lines = [line for line in source.splitlines() if ' rle(const ' in line]
    if len(lines) != 2:
        raise ValueError('expected two RLE definitions')
    return '\n'.join(lines)


def normalize(source):
    return re.sub(r'\s+', '', source.replace('std::', '').replace('inline ', ''))


def main():
    entries = shared.parse_jsonc((ROOT / 'cpp.json').read_text(encoding='utf-8'))
    header = definitions((ROOT / HEADER).read_text(encoding='utf-8'))
    source = (ROOT / 'verify/local/run_length_encoding.test.cpp').read_text(encoding='utf-8')
    include = '#include "../../' + HEADER + '"'
    if source.count(include) != 1:
        raise ValueError('test include not found uniquely')
    with tempfile.TemporaryDirectory(prefix='rle-snippet-') as directory:
        for key, prefix in [('default', 'cpptemplate'), ('paiza', 'paizatemplate')]:
            entry = entries[key]
            if entry.get('prefix') != prefix:
                raise ValueError('template prefix changed')
            snippet = definitions('\n'.join(entry['body']))
            if normalize(snippet) != normalize(header):
                raise ValueError(f'{prefix} RLE differs from header')
            path, binary = Path(directory) / 'test.cpp', Path(directory) / 'test'
            path.write_text(source.replace(include, '#include <bits/stdc++.h>\nusing namespace std;\n' + snippet), encoding='utf-8')
            subprocess.run(shlex.split(os.environ.get('CXX', 'g++')) + ['-std=c++20', '-O2', str(path), '-o', str(binary)], check=True)
            subprocess.run([str(binary)], check=True)
    print('RLE: both templates match header and pass tests')


if __name__ == '__main__':
    main()
