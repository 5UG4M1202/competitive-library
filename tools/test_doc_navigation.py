#!/usr/bin/env python3
"""Check links between sibling library docs in generated verifier pages."""
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
LAYOUT = ROOT / '.competitive-verifier/docs/static/_layouts/page.html'
INDEX = ROOT / '.competitive-verifier/docs/index.md'


def main():
    layout = LAYOUT.read_text(encoding='utf-8')
    pairs = re.findall(r'''replace:\s*'href="([^"']+)"',\s*'href="([^"']+)"' ''', layout + ' ')
    rewrites = dict(pairs)
    checked = 0
    for doc in (ROOT / 'library').rglob('*.md'):
        if doc.name == 'README.md':
            continue
        for link in re.findall(r'\]\(([^)]+\.md)\)', doc.read_text(encoding='utf-8')):
            if '/' in link or ':' in link or link.startswith('#'):
                continue
            expected = link[:-3] + '.hpp.html'
            if not (doc.parent / (link[:-3] + '.hpp')).is_file():
                raise ValueError(f'{doc.relative_to(ROOT)}: missing header for {link}')
            if rewrites.get(link) != expected:
                raise ValueError(f'{doc.relative_to(ROOT)}: missing site rewrite {link} -> {expected}')
            checked += 1
    if not checked:
        raise ValueError('no sibling documentation links checked')
    index = INDEX.read_text(encoding='utf-8')
    index_links = re.findall(r"\{\{\s*'(/library/[^']+\.hpp\.html)'\s*\|\s*relative_url\s*\}\}", index)
    if not index_links:
        raise ValueError('no verifier index library links checked')
    for link in index_links:
        header = ROOT / link.removeprefix('/').removesuffix('.html')
        if not header.is_file() or not header.with_suffix('.md').is_file():
            raise ValueError(f'verifier index link has no matching header and documentation: {link}')
    required = ('interval_set', 'lazy_reversible_rbst', 'euler_tour',
                'distinct_k_segments', 'nqueen', 'mydate')
    for name in required:
        if not any(link.endswith(f'/{name}.hpp.html') for link in index_links):
            raise ValueError(f'verifier index is missing {name}')
    for doc in (ROOT / 'library').rglob('*.md'):
        if doc.name == 'README.md':
            continue
        for link in re.findall(r'\]\((\.\./\.\./verify/local/[^)]+\.test\.cpp)\)', doc.read_text(encoding='utf-8')):
            if not (doc.parent / link).is_file():
                raise ValueError(f'{doc.relative_to(ROOT)}: missing local test for {link}')
            if rewrites.get(link) != link + '.html':
                raise ValueError(f'{doc.relative_to(ROOT)}: missing test link rewrite for {link}')
    local_tests = list((ROOT / 'verify/local').glob('*.test.cpp'))
    if not local_tests:
        raise ValueError('no local verifier tests found')
    for test in local_tests:
        lines = test.read_text(encoding='utf-8').splitlines()
        if '// competitive-verifier: STANDALONE' not in lines or '// competitive-verifier: DISPLAY no-index' not in lines:
            raise ValueError(f'{test.relative_to(ROOT)}: local test should be verified and hidden from the index')
    print(f'Verifier site: {checked} sibling documentation links have valid rewrites')
    print(f'Verifier site: {len(index_links)} index links have matching headers and documentation')
    print(f'Verifier site: {len(local_tests)} local tests are configured as standalone and no-index')


if __name__ == '__main__':
    main()
