#!/usr/bin/env python3
"""Compile and exercise the standalone ACL-style segment tree snippets."""
from pathlib import Path
import os
import shlex
import subprocess
import sys
import tempfile

sys.dont_write_bytecode = True
import check_interval_set_snippet_sync as shared
import test_dynamic_lazy_segtree_snippet as dynamic

ROOT = Path(__file__).resolve().parents[1]
PREAMBLE = '''
#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
'''
CASES = {
    '@segtree_normal': '''
int main() {
  Segtree seg(4);
  seg.set(0, 3);
  seg.set(2, 5);
  assert(seg.prod(0, 4) == 8 && seg.prod(1, 2) == 0);
}
''',
    '@segtree_lazy': '''
int main() {
  LazySegtree seg(vector<S>(5, {0, 1}));
  seg.apply(0, 5, 2);
  seg.apply(1, 4, 3);
  assert(seg.prod(0, 5).value == 19);
  assert(seg.prod(1, 3).value == 10);
  assert(seg.prod(2, 2).value == 0);
  vector<ll> a(20, 0);
  LazySegtree random_seg(vector<S>(20, {0, 1}));
  mt19937 rng(1187);
  for (int step = 0; step < 500; ++step) {
    int l = rng() % 21, r = rng() % 21;
    if (l > r) swap(l, r);
    ll delta = static_cast<int>(rng() % 21) - 10;
    random_seg.apply(l, r, delta);
    for (int i = l; i < r; ++i) a[i] += delta;
    l = rng() % 21; r = rng() % 21;
    if (l > r) swap(l, r);
    assert(random_seg.prod(l, r).value == accumulate(a.begin() + l, a.begin() + r, 0LL));
  }
}
''',
    '@segtree_dynamic': '''
int main() {
  constexpr ll N = 3000000000LL;
  DynamicSegtree seg(0, N, init);
  assert(seg.all_prod().value == N);
  seg.apply(5, 10, 0LL);
  assert(seg.prod(0, 12).value == 7);
  seg.apply(6, 9, 2LL);
  assert(seg.prod(0, 12).value == 13);
  assert(seg.all_prod().value == N + 1);
  seg.apply(0, 1, 8000000000000000000LL);
  assert(seg.prod(0, 1).value == 8000000000000000000LL);
  seg.apply(0, 1, nullopt);
  assert(seg.prod(0, 1).value == 8000000000000000000LL);
  DynamicSegtree random_seg(0, 20, init);
  vector<ll> a(20, 1);
  mt19937 rng(9917);
  for (int step = 0; step < 500; ++step) {
    int l = rng() % 21, r = rng() % 21;
    if (l > r) swap(l, r);
    ll value = static_cast<int>(rng() % 21) - 10;
    random_seg.apply(l, r, value);
    for (int i = l; i < r; ++i) a[i] = value;
    l = rng() % 21; r = rng() % 21;
    if (l > r) swap(l, r);
    assert(random_seg.prod(l, r).value == accumulate(a.begin() + l, a.begin() + r, 0LL));
  }
}
''',
}


def main():
    entries = shared.parse_jsonc((ROOT / 'cpp.json').read_text(encoding='utf-8'))
    cpptemplate = '\n'.join(entries['default']['body'])
    compiler = shlex.split(os.environ.get('CXX', 'g++'))
    with tempfile.TemporaryDirectory(prefix='segtree-examples-') as directory:
        for name, test in CASES.items():
            entry = entries[name]
            if entry['prefix'] != name:
                raise ValueError(f'{name}: unexpected prefix')
            body = '\n'.join(entry['body'])
            extra = dynamic.definition(cpptemplate) if name == '@segtree_dynamic' else ''
            path = Path(directory) / (name[1:] + '.cpp')
            binary = Path(directory) / name[1:]
            path.write_text(PREAMBLE + '\n' + extra + '\n' + body + '\n' + test, encoding='utf-8')
            subprocess.run(compiler + ['-std=c++20', '-O2', '-Wall', '-Wextra', '-Werror=narrowing', str(path), '-o', str(binary)], check=True)
            subprocess.run([str(binary)], check=True)
            print(f'{name}: compiled and passed')


if __name__ == '__main__':
    main()
