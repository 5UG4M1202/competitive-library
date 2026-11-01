#!/usr/bin/env python3
"""Compare and test the independent combination implementation in both templates."""
from pathlib import Path
import os
import shlex
import subprocess
import sys
import tempfile

sys.dont_write_bytecode = True
import check_interval_set_snippet_sync as shared

ROOT = Path(__file__).resolve().parents[1]
MARKER = 'template<class Iterator>\nbool next_combination('


def main():
    entries = shared.parse_jsonc((ROOT / 'cpp.json').read_text(encoding='utf-8'))
    implementations = []
    for key in ('default', 'paiza'):
        body = '\n'.join(entries[key]['body'])
        if body.count(MARKER) != 1:
            raise ValueError(f'{key}: expected one next_combination definition')
        implementations.append(shared.extract_function(body, MARKER))
    if implementations[0] != implementations[1]:
        raise ValueError('next_combination differs between templates')

    fixture = (ROOT / 'tools/fixtures/next_combination.test.cpp').read_text(encoding='utf-8')
    marker = '// NEXT_COMBINATION_IMPLEMENTATION'
    if fixture.count(marker) != 1:
        raise ValueError('missing test fixture marker')
    with tempfile.TemporaryDirectory(prefix='next-combination-') as directory:
        path = Path(directory) / 'test.cpp'
        binary = Path(directory) / 'test'
        path.write_text(fixture.replace(marker, implementations[0]), encoding='utf-8')
        subprocess.run(shlex.split(os.environ.get('CXX', 'g++')) +
                       ['-std=c++20', '-O2', str(path), '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True)
    print('both templates match and next_combination passes exhaustive tests')


if __name__ == '__main__':
    main()
