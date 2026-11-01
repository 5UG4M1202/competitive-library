#!/usr/bin/env python3
"""Check both templates against the standalone number and division helpers."""
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
GROUPS = [
    ('trial_division', ['is_prime', 'prime_factor', 'sieve_of_era', 'divisor']),
    ('integer_division', ['ceil_div', 'floor_div', 'out_div']),
    ('integer_power', ['_pow']),
    ('manhattan_distance', ['mnht']),
]


def definitions(source, names):
    found = {}
    for line in source.splitlines():
        for name in names:
            if re.match(r'^(?:inline )?(?:constexpr )?.*\b' + name + r'\(', line):
                if name in found:
                    raise ValueError(f'duplicate function: {name}')
                found[name] = line
    if set(found) != set(names):
        raise ValueError(f'missing definitions: {set(names) - set(found)}')
    return found


def normalize(source):
    source = source.replace('std::', '').replace('long long', 'll')
    source = re.sub(r'\binline\s+', '', source)
    return re.sub(r'\s+', '', source)


def main():
    entries = shared.parse_jsonc((ROOT / 'cpp.json').read_text(encoding='utf-8'))
    with tempfile.TemporaryDirectory(prefix='math-helpers-') as directory:
        for component, names in GROUPS:
            snippet_only = component in ('integer_power', 'manhattan_distance')
            header_path = f'library/math/{component}.hpp'
            header = None if snippet_only else definitions((ROOT / header_path).read_text(encoding='utf-8'), names)
            test_path = f'tools/fixtures/{component}.cpp' if snippet_only else f'verify/local/{component}.test.cpp'
            test = (ROOT / test_path).read_text(encoding='utf-8')
            include = '// @snippet-definition@' if snippet_only else '#include "../../' + header_path + '"'
            if test.count(include) != 1:
                raise ValueError('test include not found uniquely')
            for key, prefix in [('default', 'cpptemplate'), ('paiza', 'paizatemplate')]:
                entry = entries[key]
                if entry.get('prefix') != prefix:
                    raise ValueError('template prefix changed')
                body = '\n'.join(line for line in entry['body'] if not line.startswith(' '))
                snippet = definitions(body, names)
                for name in names:
                    if header is not None and normalize(header[name]) != normalize(snippet[name]):
                        raise ValueError(f'{prefix} {name} differs from header')
                preamble = '#include <bits/stdc++.h>\nusing namespace std;\nusing ll = long long;\n'
                path, binary = Path(directory) / 'test.cpp', Path(directory) / 'test'
                path.write_text(test.replace(include, preamble + '\n'.join(snippet[n] for n in names)), encoding='utf-8')
                subprocess.run(shlex.split(os.environ.get('CXX', 'g++')) + ['-std=c++20', '-O2', str(path), '-o', str(binary)], check=True)
                subprocess.run([str(binary)], check=True)
    print('math helpers: both template expansions pass tests; maintained headers match')


if __name__ == '__main__':
    main()
