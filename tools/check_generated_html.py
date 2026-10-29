#!/usr/bin/env python3
"""Check local href/src targets in a generated Jekyll site.

Usage: python3 tools/check_generated_html.py SITE_DIR [BASEURL]
External URLs and fragment-only links are not fetched.
"""

from html.parser import HTMLParser
from pathlib import Path
from urllib.parse import unquote, urljoin, urlsplit
import sys


class Links(HTMLParser):
    def __init__(self):
        super().__init__()
        self.targets = []

    def handle_starttag(self, tag, attributes):
        for name, value in attributes:
            if name in ('href', 'src') and value:
                self.targets.append(value)


def main():
    if len(sys.argv) not in (2, 3):
        raise SystemExit('usage: check_generated_html.py SITE_DIR [BASEURL]')
    root = Path(sys.argv[1]).resolve()
    baseurl = '/' + sys.argv[2].strip('/') if len(sys.argv) == 3 and sys.argv[2].strip('/') else ''
    failures = []
    checked = 0
    pages = sorted(root.rglob('*.html'))
    if not pages:
        raise SystemExit('no HTML pages found')
    for page in pages:
        parser = Links()
        parser.feed(page.read_text(encoding='utf-8'))
        page_url = baseurl + '/' + page.relative_to(root).as_posix()
        for value in parser.targets:
            parsed = urlsplit(value)
            if parsed.scheme or parsed.netloc or not parsed.path:
                continue
            path = unquote(urlsplit(urljoin(page_url, value)).path)
            if baseurl and not (path == baseurl or path.startswith(baseurl + '/')):
                failures.append((page, value, 'outside baseurl'))
                continue
            relative = path[len(baseurl):].lstrip('/')
            target = (root / relative).resolve()
            if not target.is_relative_to(root):
                failures.append((page, value, 'outside site'))
                continue
            if target.is_dir():
                target = target / 'index.html'
            checked += 1
            if not target.is_file():
                failures.append((page, value, 'not found'))
    for page, value, reason in failures:
        print(f'{page.relative_to(root)}: {value}: {reason}', file=sys.stderr)
    if failures:
        raise SystemExit(f'{len(failures)} broken local links or assets')
    print(f'checked {checked} local links/assets in {len(pages)} HTML pages')


if __name__ == '__main__':
    main()
