#!/usr/bin/env python3
"""Run real template input/output expansion tests."""
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


def outputs(source):
    lines = [line for line in source.splitlines() if re.match(r'^template <class T> (?:inline )?void out_c(?:_pairs)?\(', line)]
    if len(lines) != 2:
        raise ValueError('expected both output helpers')
    return '\n'.join(lines)


def inputs(source):
    start = source.index('inline void scan() {}')
    marker = 'template<class H,class...T> inline void in(H &h,T&...t){scan(h);in(t...);}'
    end = source.index(marker, start) + len(marker)
    return source[start:end]


def normalize(source):
    source = re.sub(r'/\*.*?\*/|//[^\n]*', '', source, flags=re.DOTALL)
    source = re.sub(r'\binline\s+', '', source.replace('std::', ''))
    return re.sub(r'\s+', '', source)


def main():
    entries = shared.parse_jsonc((ROOT / 'cpp.json').read_text(encoding='utf-8'))
    body = '\n'.join(entries['default']['body'])
    if entries['default'].get('prefix') != 'cpptemplate' or entries['paiza'].get('prefix') != 'paizatemplate':
        raise ValueError('template prefix changed')
    snippet_in, snippet_out = inputs(body), outputs(body)
    if normalize(outputs('\n'.join(entries['paiza']['body']))) != normalize(snippet_out):
        raise ValueError('paizatemplate output differs from cpptemplate')
    source = (ROOT / 'tools/fixtures/template_io.cpp').read_text(encoding='utf-8')
    include = '// @snippet-definition@'
    if source.count(include) != 1:
        raise ValueError('test include not found uniquely')
    with tempfile.TemporaryDirectory(prefix='template-io-') as directory:
        path, binary = Path(directory) / 'test.cpp', Path(directory) / 'test'
        preamble = '#include <bits/stdc++.h>\nusing namespace std;\n'
        path.write_text(source.replace(include, preamble + snippet_out + '\n' + snippet_in), encoding='utf-8')
        subprocess.run(shlex.split(os.environ.get('CXX', 'g++')) + ['-std=c++20', '-O2', str(path), '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True)
    print('template I/O: expanded snippets pass nested input/output tests')


if __name__ == '__main__':
    main()
