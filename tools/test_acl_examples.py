#!/usr/bin/env python3
"""Compile and execute the standalone examples in the ACL guide."""
import os
from pathlib import Path
import re
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
guide = root / '.competitive-verifier/docs/static/acl-usage.md'
examples = re.findall(r'```cpp\s*\n(.*?)\n```', guide.read_text(encoding='utf-8'), re.S)
if len(examples) != 3:
    raise SystemExit(f'expected 3 ACL examples, found {len(examples)}')
acl = os.environ.get('ACL_INCLUDE_PATH')
if not acl:
    raise SystemExit('set ACL_INCLUDE_PATH to the AtCoder Library root')
with tempfile.TemporaryDirectory() as temp:
    for i, example in enumerate(examples):
        source = Path(temp) / f'example{i}.cpp'
        binary = Path(temp) / f'example{i}'
        source.write_text(example, encoding='utf-8')
        subprocess.run(['g++', '-std=c++20', '-Wall', '-Wextra', '-I', acl,
                        str(source), '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True)
print('ACL guide examples: 3 passed')
