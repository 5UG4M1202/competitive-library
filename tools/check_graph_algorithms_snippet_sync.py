#!/usr/bin/env python3
"""Check Graph, Dijkstra and Kruskal in both templates against their headers."""
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


def graph_definition(source):
    pos = source.index('struct Edge {')
    start = source.rfind('template <typename T = int>', 0, pos)
    if start < 0:
        raise ValueError('Edge template missing')
    alias = re.search(r'using Edges\s*=\s*(?:std::)?vector<Edge<T>\s*>\s*;', source[pos:])
    if not alias:
        raise ValueError('Edges alias missing')
    return source[start:pos + alias.end()]


def function_definition(source, name):
    pos = source.index(name + '(Graph<T> &g,')
    start = source.rfind('template <typename T>', 0, pos)
    if start < 0:
        raise ValueError(f'{name} template missing')
    return source[start:source.index('\n}', pos) + 2]


def normalize(source):
    source = re.sub(r'/\*.*?\*/|//[^\n]*', '', source, flags=re.DOTALL)
    source = source.replace('ALL(edges)', 'edges.begin(), edges.end()')
    source = source.replace('std::', '').replace('atcoder::', '')
    return re.sub(r'\s+', '', source)


def main():
    entries = shared.parse_jsonc((ROOT / 'cpp.json').read_text(encoding='utf-8'))
    entry = entries['default']
    if entry.get('prefix') != 'cpptemplate':
        raise ValueError('cpptemplate prefix changed')
    body = '\n'.join(entry['body'])
    if 'Graph comparison: ei1333/library, Unlicense' not in body:
        raise ValueError('graph comparison-source notice missing')
    graph = graph_definition(body)
    dijkstra = function_definition(body, 'dijkstra')
    kruskal = function_definition(body, 'kruskal')
    for label, snippet, header in (
        ('Graph', graph, 'library/graph/graph.hpp'),
        ('dijkstra', dijkstra, 'library/graph/dijkstra.hpp'),
        ('kruskal', kruskal, 'library/graph/kruskal.hpp'),
    ):
        text = (ROOT / header).read_text(encoding='utf-8')
        target = graph_definition(text) if label == 'Graph' else function_definition(text, label)
        if normalize(snippet) != normalize(target):
            raise ValueError(f'{label} snippet and header differ')
    source = (ROOT / 'verify/local/graph_algorithms.test.cpp').read_text(encoding='utf-8')
    a = '#include "../../library/graph/dijkstra.hpp"'
    b = '#include "../../library/graph/kruskal.hpp"'
    if source.count(a) != 1 or source.count(b) != 1:
        raise ValueError('test includes not found uniquely')
    preamble = ('#include <bits/stdc++.h>\n#include <atcoder/dsu>\n'
                'using namespace std;\nusing namespace atcoder;\n'
                '#define ALL(x) (x).begin(), (x).end()\n')
    source = source.replace(a, preamble + graph + '\n' + dijkstra + '\n' + kruskal).replace(b, '')
    with tempfile.TemporaryDirectory(prefix='graph-snippet-') as directory:
        path = Path(directory) / 'test.cpp'
        binary = Path(directory) / 'test'
        path.write_text(source, encoding='utf-8')
        subprocess.run(shlex.split(os.environ.get('CXX', 'g++')) +
                       ['-std=c++20', '-O2', str(path), '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True)
        paiza = entries['paiza']
        if paiza.get('prefix') != 'paizatemplate':
            raise ValueError('paizatemplate prefix changed')
        body = '\n'.join(paiza['body'])
        if 'Graph comparison: ei1333/library, Unlicense' not in body or 'AtCoder Library dsu v1.5.1' not in body:
            raise ValueError('paiza comparison-source notice missing')
        paiza_graph = graph_definition(body)
        paiza_dijkstra = function_definition(body, 'dijkstra')
        paiza_kruskal = function_definition(body, 'kruskal')
        for label, expected, actual in (('Graph', graph, paiza_graph),
                                        ('dijkstra', dijkstra, paiza_dijkstra),
                                        ('kruskal', kruskal, paiza_kruskal)):
            if normalize(expected) != normalize(actual):
                raise ValueError(f'paizatemplate {label} differs from header')
        start = body.index('struct dsu {')
        embedded_dsu = body[start:body.index('\n};', start) + 3]
        original = (ROOT / 'verify/local/graph_algorithms.test.cpp').read_text(encoding='utf-8')
        preamble = '#include <bits/stdc++.h>\nusing namespace std;\n#define ALL(x) (x).begin(), (x).end()\n'
        paiza_code = preamble + paiza_graph + '\n' + paiza_dijkstra + '\n' + embedded_dsu + '\n' + paiza_kruskal
        fixture = original.replace(a, paiza_code).replace(b, '')
        fixture = fixture.replace('int main() {', 'int main() { dsu u(3); u.merge(0, 1); assert(u.same(0, 1) && !u.same(0, 2) && u.size(0) == 2 && u.groups().size() == 2);', 1)
        path.write_text(fixture, encoding='utf-8')
        subprocess.run(shlex.split(os.environ.get('CXX', 'g++')) +
                       ['-std=c++20', '-O2', str(path), '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True)
    print('Graph, dijkstra, kruskal: both templates match headers and pass tests')


if __name__ == '__main__':
    main()
