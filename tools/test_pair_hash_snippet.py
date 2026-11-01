#!/usr/bin/env python3
"""Compile and exercise the pairhash snippet expansion."""
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
    marker = 'struct phash {'
    if source.count(marker) != 1:
        raise ValueError('phash must occur exactly once')
    start = source.index(marker)
    return source[start:source.index('\n};', start) + 3]


def main():
    entry = shared.parse_jsonc((ROOT / 'cpp.json').read_text(encoding='utf-8'))['pairhash']
    if entry.get('prefix') != 'pairhash':
        raise ValueError('pairhash prefix changed')
    body = '\n'.join(entry['body'])
    if 'qiita.com/tqk/items/7c4df1eeb5ebaedcf714' not in body:
        raise ValueError('comparison source notice missing')
    definition(body)
    expanded = body.replace('${1:seen}', 'seen')
    if '${' in expanded or not expanded.rstrip().endswith('seen;'):
        raise ValueError('unexpected placeholder or incomplete declaration')
    source = (ROOT / 'tools/fixtures/pair_hash.cpp').read_text(encoding='utf-8')
    include = '// @snippet-definition@'
    if source.count(include) != 1:
        raise ValueError('test include not found uniquely')
    with tempfile.TemporaryDirectory(prefix='pairhash-snippet-') as directory:
        path, binary = Path(directory) / 'test.cpp', Path(directory) / 'test'
        preamble = '#include <bits/stdc++.h>\nusing namespace std;\nusing ll = long long;\n'
        path.write_text(source.replace(include, preamble + expanded), encoding='utf-8')
        subprocess.run(shlex.split(os.environ.get('CXX', 'g++')) + ['-std=c++20', '-O2', str(path), '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True)
    print('pairhash: expanded declaration passes tests')


if __name__ == '__main__':
    main()
