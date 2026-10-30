#!/usr/bin/env python3
"""Serve a built project site on localhost, preserving its base URL."""
import argparse
from functools import partial
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from urllib.parse import unquote, urlsplit


class SiteHandler(SimpleHTTPRequestHandler):
    def __init__(self, *args, root, prefix, **kwargs):
        self.root = root
        self.prefix = prefix
        super().__init__(*args, directory=str(root), **kwargs)

    def send_head(self):
        raw = unquote(urlsplit(self.path).path)
        if not raw.startswith(self.prefix + '/'):
            self.send_error(404)
            return None
        relative = raw[len(self.prefix) + 1:]
        parts = relative.split('/')
        if any(part in ('.', '..') or '\\' in part or ':' in part or '\x00' in part for part in parts):
            self.send_error(404)
            return None
        target = self.root.joinpath(*parts).resolve()
        if not target.is_relative_to(self.root):
            self.send_error(404)
            return None
        if target.is_dir():
            target = (target / 'index.html').resolve()
        if not target.is_relative_to(self.root) or not target.is_file():
            self.send_error(404)
            return None
        try:
            file = target.open('rb')
        except OSError:
            self.send_error(404)
            return None
        self.send_response(200)
        self.send_header('Content-Type', self.guess_type(str(target)))
        self.send_header('Content-Length', str(target.stat().st_size))
        self.end_headers()
        return file


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('site', type=Path, help='generated HTML directory')
    parser.add_argument('--baseurl', default='/competitive-library')
    parser.add_argument('--port', type=int, default=8765)
    args = parser.parse_args()
    root = args.site.resolve()
    if not root.is_dir() or not (root / 'index.html').is_file():
        parser.error('site must be a directory containing index.html')
    prefix = args.baseurl.rstrip('/')
    if not prefix.startswith('/') or len(prefix) < 2 or any(c not in 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_' for c in prefix[1:]):
        parser.error('baseurl must be a single URL path segment')
    if not 1 <= args.port <= 65535:
        parser.error('port must be between 1 and 65535')
    handler = partial(SiteHandler, root=root, prefix=prefix)
    with ThreadingHTTPServer(('127.0.0.1', args.port), handler) as server:
        print(f'Preview: http://127.0.0.1:{args.port}{prefix}/', flush=True)
        print('Stop with Ctrl+C.', flush=True)
        try:
            server.serve_forever()
        except KeyboardInterrupt:
            print('\nStopped.')


if __name__ == '__main__':
    main()
