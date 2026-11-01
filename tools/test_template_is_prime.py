#!/usr/bin/env python3
"""Check the simple trial-division primality helpers in both templates."""
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
    with tempfile.TemporaryDirectory(prefix='template-is-prime-') as directory:
        for key, prefix in [('default', 'cpptemplate'), ('paiza', 'paizatemplate')]:
            entry = entries[key]
            if entry.get('prefix') != prefix:
                raise ValueError('template prefix changed')
            lines = [line for line in entry['body'] if 'bool is_prime(ll x)' in line]
            if len(lines) != 1:
                raise ValueError(f'{prefix} is_prime missing or duplicated')
            source = '''#include <cassert>
#include <climits>
using ll = long long;
''' + lines[0] + '''
int main() {
  assert(!is_prime(LLONG_MIN) && !is_prime(-1) && !is_prime(0) && !is_prime(1));
  assert(is_prime(2) && is_prime(3) && !is_prime(4));
  assert(!is_prime(LLONG_MAX));
  for (int n = 2; n < 20000; ++n) {
    bool expected = true;
    for (int d = 2; d < n; ++d) if (n % d == 0) { expected = false; break; }
    assert(is_prime(n) == expected);
  }
}
'''
            path, binary = Path(directory) / 'test.cpp', Path(directory) / 'test'
            path.write_text(source, encoding='utf-8')
            subprocess.run(shlex.split(os.environ.get('CXX', 'g++')) + ['-std=c++20', '-O2', str(path), '-o', str(binary)], check=True)
            subprocess.run([str(binary)], check=True)
    print('is_prime: both templates pass boundary and comparison tests')


if __name__ == '__main__':
    main()
