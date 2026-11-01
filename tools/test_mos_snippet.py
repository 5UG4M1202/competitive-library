#!/usr/bin/env python3
"""Exercise @mos with 1-based closed input and range-sum callbacks."""
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
    entry = shared.parse_jsonc((ROOT / 'cpp.json').read_text(encoding='utf-8'))['@Mos']
    if entry.get('prefix') != '@mos':
        raise ValueError('@mos prefix changed')
    body = '\n'.join(entry['body'])
    callbacks = {
        '// lを下げるときの追加処理をここに書く': 'ret += values[l];',
        '// rを上げるとき、要素 r を追加する処理をここに書く': 'ret += values[r];',
        '// lを上げるときの追加処理をここに書く': 'ret -= values[l];',
        '// rを下げるとき、要素 r を削除する処理をここに書く': 'ret -= values[r];',
    }
    for marker, code in callbacks.items():
        if body.count(marker) != 1:
            raise ValueError(f'missing or duplicate callback marker: {marker}')
        body = body.replace(marker, code)
    source = '''#include <bits/stdc++.h>
using namespace std;
using ll = long long;
int main() {
  mt19937 rng(9157);
  for (int N = 0; N <= 40; ++N) {
    for (int trial = 0; trial < 25; ++trial) {
      ll Q = N == 0 ? 0 : 100;
      vector<ll> values(N);
      for (auto& x : values) x = static_cast<int>(rng() % 101) - 50;
      vector<ll> expected;
      ostringstream input;
      for (int i = 0; i < Q; ++i) {
        int l = static_cast<int>(rng() % N);
        int r = l + 1 + static_cast<int>(rng() % (N - l));
        input << l + 1 << ' ' << r << '\\n';
        expected.push_back(accumulate(values.begin() + l, values.begin() + r, 0LL));
      }
      istringstream stream(input.str());
      auto* original = cin.rdbuf(stream.rdbuf());
''' + body + '''
      cin.rdbuf(original);
      assert(ans == expected);
    }
  }
}
'''
    with tempfile.TemporaryDirectory(prefix='mos-snippet-') as directory:
        path, binary = Path(directory) / 'test.cpp', Path(directory) / 'test'
        path.write_text(source, encoding='utf-8')
        subprocess.run(shlex.split(os.environ.get('CXX', 'g++')) + ['-std=c++20', '-O2', str(path), '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True)
    print('@mos: 1-based closed range-sum expansion passes randomized and zero-query tests')


if __name__ == '__main__':
    main()
