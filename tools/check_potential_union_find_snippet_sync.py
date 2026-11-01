#!/usr/bin/env python3
"""Check the cpptemplate weighted union-find against its maintained header."""
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
HEADER = 'library/data_structure/union_find_with_potential.hpp'
TEST = ROOT / 'verify/local/union_find_with_potential.test.cpp'
MARKER = 'template <class T>\nstruct UnionFindWithPotential {'


def definition(text):
    if text.count(MARKER) != 1:
        raise ValueError('weighted union-find definition must occur exactly once')
    start = text.index(MARKER)
    end = text.index('\n};', start) + len('\n};')
    return text[start:end]


def normalize(text):
    text = re.sub(r'/\*.*?\*/|//[^\n]*', '', text, flags=re.DOTALL)
    return re.sub(r'\s+', '', text.replace('std::', ''))


def main():
    entry = shared.parse_jsonc((ROOT / 'cpp.json').read_text(encoding='utf-8'))['default']
    if entry['prefix'] != 'cpptemplate':
        raise ValueError('cpptemplate prefix changed')
    body = '\n'.join(entry['body'])
    start = body.index(MARKER)
    notice = body[max(0, start - 1500):start]
    if 'NyaanNyaan/library' not in notice or 'CC0-1.0' not in notice:
        raise ValueError('weighted union-find provenance notice missing')
    snippet = definition(body)
    if normalize(snippet) != normalize(definition((ROOT / HEADER).read_text(encoding='utf-8'))):
        raise ValueError('cpptemplate weighted union-find differs from its header')
    source = TEST.read_text(encoding='utf-8')
    include = '#include "../../' + HEADER + '"'
    if source.count(include) != 1:
        raise ValueError('test include not found uniquely')
    replacement = '#include <utility>\n#include <vector>\nusing namespace std;\n' + snippet
    with tempfile.TemporaryDirectory(prefix='potential-uf-sync-') as directory:
        path = Path(directory) / 'snippet.cpp'
        binary = Path(directory) / 'snippet-test'
        path.write_text(source.replace(include, replacement), encoding='utf-8')
        subprocess.run(shlex.split(os.environ.get('CXX', 'g++')) +
                       ['-std=c++20', '-O2', str(path), '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True)
    print('cpptemplate weighted union-find matches header and passes tests')


if __name__ == '__main__':
    main()
