#!/usr/bin/env python3
"""Compare the edit-distance function in cpptemplate and its header."""
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
HEADER = 'library/string/levenstein_distance.hpp'
MARKER = 'int levenstein_distance('


def definition(source):
    if source.count(MARKER) != 1:
        raise ValueError('edit-distance definition must occur exactly once')
    start = source.index(MARKER)
    return source[start:source.index('\n}', start) + 2]


def normalize(source):
    source = re.sub(r'/\*.*?\*/|//[^\n]*', '', source, flags=re.DOTALL)
    return re.sub(r'\s+', '', source.replace('std::', ''))


def main():
    entry = shared.parse_jsonc((ROOT / 'cpp.json').read_text(encoding='utf-8'))['default']
    if entry.get('prefix') != 'cpptemplate':
        raise ValueError('cpptemplate prefix changed')
    snippet = definition('\n'.join(entry['body']))
    if normalize(snippet) != normalize(definition((ROOT / HEADER).read_text(encoding='utf-8'))):
        raise ValueError('edit-distance snippet and header differ')
    source = (ROOT / 'verify/local/levenstein_distance.test.cpp').read_text(encoding='utf-8')
    include = '#include "../../' + HEADER + '"'
    if source.count(include) != 1:
        raise ValueError('test include not found uniquely')
    preamble = '#include <algorithm>\n#include <string>\n#include <vector>\nusing namespace std;\n'
    with tempfile.TemporaryDirectory(prefix='edit-distance-snippet-') as directory:
        path = Path(directory) / 'test.cpp'
        binary = Path(directory) / 'test'
        path.write_text(source.replace(include, preamble + snippet), encoding='utf-8')
        subprocess.run(shlex.split(os.environ.get('CXX', 'g++')) +
                       ['-std=c++20', '-O2', str(path), '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True)
    print('levenstein_distance: snippet matches header and passes tests')


if __name__ == '__main__':
    main()
