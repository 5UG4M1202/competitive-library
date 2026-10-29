#!/usr/bin/env python3
"""Compare AI-assisted maintained snippets with their headers and tests."""

from pathlib import Path
import os
import shlex
import subprocess
import sys
import tempfile

sys.dont_write_bytecode = True
import check_interval_set_snippet_sync as shared

ROOT = Path(__file__).resolve().parents[1]
ITEMS = {
    '@mydate': ('library/misc/mydate.hpp', 'verify/local/mydate.test.cpp'),
    '@DistinctMax': ('library/data_structure/distinct_k_segments.hpp', 'verify/local/distinct_k_segments.test.cpp'),
    '@NQUEEN': ('library/data_structure/nqueen.hpp', 'verify/local/nqueen.test.cpp'),
}


def main():
    entries = shared.parse_jsonc((ROOT / 'cpp.json').read_text(encoding='utf-8'))
    with tempfile.TemporaryDirectory(prefix='legacy-snippet-sync-') as directory:
        for name, (header, test) in ITEMS.items():
            entry = entries[name]
            if entry['prefix'] != name:
                raise ValueError(f'{name}: unexpected prefix')
            snippet = '\n'.join(entry['body']).strip('\n')
            expected = (ROOT / header).read_text(encoding='utf-8').removeprefix('#pragma once\n').strip('\n')
            if snippet != expected:
                raise ValueError(f'{name}: differs from {header}')
            for notice in ('user reports', 'AI assistance', 'Apache-2.0', 'THIRD_PARTY_NOTICES.md'):
                if notice not in snippet:
                    raise ValueError(f'{name}: missing provenance notice: {notice}')
            test_source = (ROOT / test).read_text(encoding='utf-8')
            include = '#include "../../' + header + '"'
            if test_source.count(include) != 1:
                raise ValueError(f'{name}: test include not found uniquely')
            source = Path(directory) / (name[1:] + '.cpp')
            binary = Path(directory) / name[1:]
            source.write_text(test_source.replace(include, snippet), encoding='utf-8')
            compiler = shlex.split(os.environ.get('CXX', 'g++'))
            subprocess.run(compiler + ['-std=c++20', '-O2', str(source), '-o', str(binary)], check=True)
            subprocess.run([str(binary)], check=True)
            print(f'{name}: header matches; representative test passed')


if __name__ == '__main__':
    main()
