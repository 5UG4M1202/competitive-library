#!/usr/bin/env python3
"""Compile the binary-search example and test its boundary behavior."""
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
    entry = shared.parse_jsonc((ROOT / 'cpp.json').read_text(encoding='utf-8'))['meguru']
    if entry.get('prefix') != '@meguru':
        raise ValueError('@meguru prefix changed')
    snippet = '\n'.join(entry['body'])
    marker = '${1:/*bool*/}'
    if snippet.count(marker) != 1:
        raise ValueError('predicate placeholder missing or duplicated')
    snippet = snippet.replace(marker, 'mid >= threshold')
    source = '''#include <cassert>
#include <limits>
#include <numeric>
#include <vector>
using namespace std;
using ll = long long;
ll search(ll N, ll threshold) {
''' + snippet + '''
  return ok;
}
int main() {
  for (ll n = 0; n <= 1000; ++n)
    for (ll t = 0; t <= n; ++t) assert(search(n, t) == t);
  ll hi = numeric_limits<ll>::max();
  for (ll t : vector<ll>{0, 1, 2, hi / 2, hi - 1, hi}) assert(search(hi, t) == t);
}
'''
    with tempfile.TemporaryDirectory(prefix='meguru-snippet-') as directory:
        path, binary = Path(directory) / 'test.cpp', Path(directory) / 'test'
        path.write_text(source, encoding='utf-8')
        subprocess.run(shlex.split(os.environ.get('CXX', 'g++')) +
                       ['-std=c++20', '-O2', str(path), '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True)
    print('@meguru: expansion passes exhaustive small and long-long boundary tests')


if __name__ == '__main__':
    main()
