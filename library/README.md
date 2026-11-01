# Library

提出コードから include するヘッダーを機能別に配置します。

用途から探す場合は [やりたいことから探す](../docs/library-guide.md) を参照してください。

## Categories

- [`data_structure/`](data_structure/README.md): データ構造。
- [`graph/`](graph/README.md): 木・グラフアルゴリズム。
- [`math/`](math/README.md): 整数・数論。
- [`string/`](string/README.md): 文字列アルゴリズム。
- [`misc/`](misc/README.md): 日付などの汎用部品。

各カテゴリの README から、利用者向けの sibling Markdown で用途・最小例・前提条件を確認できます。同じ一覧には対応するヘッダーへのリンクもあります。

新しいヘッダーは、同じディレクトリに `documentation_of` を持つ Markdown と、`verify/` 以下の検証を追加してください。競技中の短さより、依存関係が明示された単独 include 可能なヘッダーを優先します。
