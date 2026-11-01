#!/usr/bin/env python3
"""Exercise the comparison helpers and GNU ordered-set snippet."""
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


def extract(source, component):
    if component == 'chminmax':
        lines = [line for line in source.splitlines() if re.match(r'^template <typename T> bool ch(?:min|max)\(', line)]
        if len(lines) != 2:
            raise ValueError('expected chmax and chmin definitions')
        return '\n'.join(lines)
    matches = re.findall(r'template\s*<typename T>\s*using ordered_set_less\s*=\s*[^;]+;', source)
    if len(matches) != 1:
        raise ValueError('ordered_set_less alias missing or duplicated')
    return matches[0]


def main():
    entry = shared.parse_jsonc((ROOT / 'cpp.json').read_text(encoding='utf-8'))['default']
    if entry.get('prefix') != 'cpptemplate':
        raise ValueError('cpptemplate prefix changed')
    body = '\n'.join(entry['body'])
    with tempfile.TemporaryDirectory(prefix='misc-helpers-') as directory:
        for component in ['chminmax', 'ordered_set']:
            definition = extract(body, component)
            source = (ROOT / f'tools/fixtures/{component}.cpp').read_text(encoding='utf-8')
            include = '// @snippet-definition@'
            if source.count(include) != 1:
                raise ValueError('test include missing or duplicated')
            preamble = '#include <bits/stdc++.h>\n#include <ext/pb_ds/assoc_container.hpp>\n#include <ext/pb_ds/tree_policy.hpp>\nusing namespace std;\nusing namespace __gnu_pbds;\n'
            path, binary = Path(directory) / 'test.cpp', Path(directory) / 'test'
            path.write_text(source.replace(include, preamble + definition), encoding='utf-8')
            subprocess.run(shlex.split(os.environ.get('CXX', 'g++')) + ['-std=c++20', '-O2', str(path), '-o', str(binary)], check=True)
            subprocess.run([str(binary)], check=True)
    print('chmax/chmin and GNU ordered set: expanded snippets pass tests')


if __name__ == '__main__':
    main()
