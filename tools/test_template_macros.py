#!/usr/bin/env python3
"""Compile actual template macro definitions with representative input."""
from pathlib import Path
import os
import re
import shlex
import subprocess
import sys
import tempfile

sys.dont_write_bytecode = True
import check_interval_set_snippet_sync as shared
import test_template_io_snippet as io

ROOT = Path(__file__).resolve().parents[1]


def selected_defines(body, names):
    found = {}
    for line in body.splitlines():
        m = re.match(r'#define\s+(\w+)(?:\(|\s)', line)
        if m and m.group(1) in names:
            if m.group(1) in found:
                raise ValueError('duplicate macro: ' + m.group(1))
            found[m.group(1)] = line
    if set(found) != set(names):
        raise ValueError('missing macros: ' + str(set(names) - set(found)))
    return '\n'.join(found[n] for n in names)


def main():
    entries = shared.parse_jsonc((ROOT / 'cpp.json').read_text(encoding='utf-8'))
    common = ['ALL', 'YES', 'NO']
    advanced = ['ALLR', 'INF', 'dbg', 'INT', 'LL', 'ULL', 'STR', 'CHR', 'DBL', 'VEC', 'VV']
    base = '#include <bits/stdc++.h>\nusing namespace std;\nusing ll = long long;\n'
    with tempfile.TemporaryDirectory(prefix='template-macros-') as directory:
        for key, prefix in [('default', 'cpptemplate'), ('paiza', 'paizatemplate')]:
            if entries[key].get('prefix') != prefix:
                raise ValueError('template prefix changed')
            body = '\n'.join(entries[key]['body'])
            definitions = selected_defines(body, common + (advanced if key == 'default' else []))
            input_helpers = io.inputs(body) if key == 'default' else ''
            exercise = '''
  istringstream stream("2 -3 100 42 word Z 1.25 4 5 6 7 8 9 10");
  auto* previous = cin.rdbuf(stream.rdbuf());
  INT(a,b); LL(c); ULL(d); STR(s); CHR(ch); DBL(x);
  VEC(int,v,3); VV(int,m,2,2);
  cin.rdbuf(previous);
  assert(a==2 && b==-3 && c==100 && d==42 && s=="word" && ch=='Z' && x==1.25);
  assert((v == vector<int>{4,5,6}) && m[1][1] == 10);
  sort(ALLR(v)); assert(v.front()==6 && v.back()==4);
  int side = 0; (void)dbg(++side); assert(side == 0 && INF == 1000000000000000000LL);
''' if key == 'default' else ''
            source = base + input_helpers + '\n' + definitions + '''
int main() {
  vector<int> ordered{3,1,2}; sort(ALL(ordered)); assert(ordered[0]==1 && ordered[2]==3);
  ostringstream out; auto* old = cout.rdbuf(out.rdbuf()); YES; NO; cout.rdbuf(old);
  assert(out.str()=="Yes\\nNo\\n");
''' + exercise + '\n}\n'
            path, binary = Path(directory) / 'test.cpp', Path(directory) / 'test'
            path.write_text(source, encoding='utf-8')
            subprocess.run(shlex.split(os.environ.get('CXX', 'g++')) + ['-std=c++20', '-O2', str(path), '-o', str(binary)], check=True)
            subprocess.run([str(binary)], check=True)
    print('template macros: both templates pass representative expansion tests')


if __name__ == '__main__':
    main()
