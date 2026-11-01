#!/usr/bin/env python3
"""Compile both complete submission-template snippets as standalone files."""
from pathlib import Path
import os
import shlex
import subprocess
import sys
import tempfile

sys.dont_write_bytecode = True
import check_interval_set_snippet_sync as shared

ROOT = Path(__file__).resolve().parents[1]


def main():
    entries = shared.parse_jsonc((ROOT / 'cpp.json').read_text(encoding='utf-8'))
    fixtures = [
        ('default', 'cpptemplate',
         'INT(n); vector<pair<int,int>> p(n); in(p); out_c_pairs(p, ":", "|");',
         '2 1 2 3 4\n', '1:2|3:4\n'),
        ('paiza', 'paizatemplate',
         ' vector<int> v{3,1,2}; sort(ALL(v)); out_c(v, ",");',
         '7\n', '7\n1,2,3\n'),
    ]
    with tempfile.TemporaryDirectory(prefix='full-template-') as directory:
        for key, prefix, content, given, expected in fixtures:
            entry = entries[key]
            if entry.get('prefix') != prefix:
                raise ValueError('template prefix changed')
            source = '\n'.join(entry['body'])
            if source.count('${0}') != 1:
                raise ValueError(f'{prefix} main placeholder missing or duplicated')
            source = source.replace('${0}', content)
            path, binary = Path(directory) / (key + '.cpp'), Path(directory) / key
            path.write_text(source + '\n', encoding='utf-8')
            subprocess.run(shlex.split(os.environ.get('CXX', 'g++')) +
                           ['-std=c++20', '-O2', str(path), '-o', str(binary)], check=True)
            actual = subprocess.run([str(binary)], input=given, text=True, capture_output=True, check=True)
            if actual.stdout != expected:
                raise ValueError(f'{prefix} output mismatch: {actual.stdout!r}')
    print('both complete templates compile and pass representative I/O examples')


if __name__ == '__main__':
    main()
