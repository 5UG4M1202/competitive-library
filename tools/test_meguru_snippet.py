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
#include <cstdlib>
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
  // ng = -1: hi - ng must remain representable in ll.
  ll hi = numeric_limits<ll>::max() - 1;
  for (ll t : vector<ll>{0, 1, 2, hi / 2, hi - 1, hi}) assert(search(hi, t) == t);
}
'''
    with tempfile.TemporaryDirectory(prefix='meguru-snippet-') as directory:
        path, binary = Path(directory) / 'test.cpp', Path(directory) / 'test'
        path.write_text(source, encoding='utf-8')
        subprocess.run(shlex.split(os.environ.get('CXX', 'g++')) +
                       ['-std=c++20', '-O2', str(path), '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True)
        condition = 'while (abs(ok - ng) > 1) {'
        if snippet.count(condition) != 1:
            raise ValueError('the editable integer stopping width is missing')
        for epsilon in ('1e-5L', '1e-30L'):
            real = snippet.replace('ll ', 'long double ').replace(condition, f'while (abs(ok - ng) > {epsilon}) {{')
            real = real.replace('mid >= threshold', 'mid * mid >= 2')
            real_source = '''#include <cassert>
#include <cmath>
#include <numeric>
using namespace std;
int main() {
  long double N = 2;
''' + real + '''
  assert(ok * ok >= 2);
  assert(ng < 0 || ng * ng < 2);
  assert(abs(ok - sqrtl(2.0L)) < 1e-5L);
}
'''
            path.write_text(real_source, encoding='utf-8')
            subprocess.run(shlex.split(os.environ.get('CXX', 'g++')) +
                           ['-std=c++20', '-O2', str(path), '-o', str(binary)], check=True)
            subprocess.run([str(binary)], check=True, timeout=5)
    print('@meguru: integer boundaries and adjustable floating-point precision pass')


if __name__ == '__main__':
    main()
