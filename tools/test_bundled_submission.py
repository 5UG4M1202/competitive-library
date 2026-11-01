#!/usr/bin/env python3
"""Compile bundled library pages and representative single-file submissions."""
from html.parser import HTMLParser
from pathlib import Path
import os
import re
import subprocess
import sys
import tempfile


class CodeParser(HTMLParser):
    def __init__(self, code_id):
        super().__init__(convert_charrefs=True)
        self.code_id = code_id
        self.capture = False
        self.parts = []

    def handle_starttag(self, tag, attrs):
        if tag == 'pre' and dict(attrs).get('id') == self.code_id:
            self.capture = True

    def handle_endtag(self, tag):
        if tag == 'pre' and self.capture:
            self.capture = False

    def handle_data(self, data):
        if self.capture:
            self.parts.append(data)


def bundled(path, code_id='code-body-2'):
    parser = CodeParser(code_id)
    parser.feed(path.read_text(encoding='utf-8'))
    result = ''.join(parser.parts)
    if not result:
        raise ValueError(f'bundled code missing: {path}')
    return result


def normalized_example(code):
    """Compare the example statements while allowing includes and comments."""
    code = re.sub(r'^\s*#include[^\n]*', '', code, flags=re.MULTILINE)
    code = re.sub(r'//[^\n]*', '', code)
    return re.sub(r'\s+', '', code)


def documented_example(doc):
    match = re.search(r'```cpp\s*\n(.*?)\n```', doc.read_text(encoding='utf-8'), re.DOTALL)
    if not match:
        raise ValueError(f'documented C++ example missing: {doc}')
    return match.group(1)


def main():
    if len(sys.argv) != 2:
        raise SystemExit('usage: test_bundled_submission.py SITE_DIR')
    site = Path(sys.argv[1])
    acl = os.environ.get('ACL_INCLUDE_PATH')
    if not acl:
        raise SystemExit('set ACL_INCLUDE_PATH to the AtCoder Library root')
    cases = {
        'dijkstra': ('library/graph/dijkstra.hpp.html',
                     'int main() { Graph<long long> g(2); g.add_directed_edge(0, 1, 3); '
                     'auto d = dijkstra(g, 0); return d[1] != 3; }\n'),
        'euler_tour': ('library/graph/euler_tour.hpp.html',
                       'int main() { Graph<long long> g(2); g.add_edge(0, 1, 3); '
                       'EulerTour<long long, long long> t(g, 0, {1, 2}); '
                       'return t.query_edge(0, 1) != 3; }\n'),
    }
    with tempfile.TemporaryDirectory() as temp:
        root = Path(__file__).resolve().parents[1]
        headers = sorted((root / 'library').rglob('*.hpp'))
        if not headers:
            raise ValueError('no library headers found')
        for header in headers:
            relative = header.relative_to(root)
            page = site / f'{relative.as_posix()}.html'
            code = bundled(page)
            includes = re.findall(r'^\s*#\s*include\s*"([^"\n]+)"', code, re.MULTILINE)
            for include in includes:
                if (root / include).is_file() or (header.parent / include).is_file():
                    raise ValueError(f'{relative}: project include was not expanded: {include}')
            source = Path(temp) / 'library.cpp'
            source.write_text(code + '\nint main() {}\n', encoding='utf-8')
            try:
                subprocess.run(['g++', '-std=c++20', '-I', acl, '-fsyntax-only', str(source)], check=True)
            except subprocess.CalledProcessError as exc:
                raise RuntimeError(f'{relative}: bundled code failed to compile') from exc
        print(f'bundled library pages: {len(headers)} headers compile without project includes', flush=True)
        for name, (page, main_code) in cases.items():
            code = bundled(site / page)
            if '#include "graph.hpp"' in code or 'struct Graph {' not in code:
                raise ValueError(f'{name}: graph dependency was not expanded')
            if name == 'euler_tour' and '#include <atcoder/segtree>' not in code:
                raise ValueError('external ACL include unexpectedly missing')
            source = Path(temp) / f'{name}.cpp'
            binary = Path(temp) / name
            source.write_text(code + '\n' + main_code, encoding='utf-8')
            subprocess.run(['g++', '-std=c++20', '-I', acl, str(source), '-o', str(binary)], check=True)
            subprocess.run([str(binary)], check=True)
        examples = {
            'euler_example': ('library/graph/euler_tour.hpp.html', 'library/graph/euler_tour.md',
                              '0 10 90\n60\n80 110\n', 'struct Graph {'),
            'dynamic_example': ('library/data_structure/dynamic_lazy_segtree.hpp.html',
                                'library/data_structure/dynamic_lazy_segtree.md', '9\n', 'struct RangeAdd'),
        }
        for name, (page, doc, expected, dependency) in examples.items():
            code = bundled(site / page, 'code-body-example')
            if dependency not in code or '#include <atcoder/segtree>' not in code:
                raise ValueError(f'{name}: required dependency or external ACL include missing')
            if '#include "library/' in code:
                raise ValueError(f'{name}: local include was not expanded')
            if normalized_example(documented_example(root / doc)) not in normalized_example(code):
                raise ValueError(f'{name}: copyable code differs from documented example')
            source = Path(temp) / f'{name}.cpp'
            binary = Path(temp) / name
            source.write_text(code, encoding='utf-8')
            subprocess.run(['g++', '-std=c++20', '-I', acl, str(source), '-o', str(binary)], check=True)
            result = subprocess.run([str(binary)], check=True, text=True, capture_output=True)
            if result.stdout != expected:
                raise ValueError(f'{name}: unexpected output {result.stdout!r}')
    print('bundled libraries and complete EulerTour/dynamic segtree examples passed')


if __name__ == '__main__':
    main()
