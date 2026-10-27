# competitive-library

競技プログラミング用 C++ ヘッダーと、その仕様・検証結果をまとめます。実装説明は各ヘッダーの sibling Markdown にある `documentation_of` から生成されます。

## 収録内容

- `IntervalSet`: 値付き半開区間 `[l, r)` の上書き・分割・結合

## 検証方針

- 非負端点の intended correctness は、境界テストと seed 固定のランダム比較で検証します。
- 現在は `STANDALONE` テストのみです。IntervalSet に自然に適合する judge-backed problem は未選定です。
- 負端点、異なる値が隣接する時の一般化された `get_mex`、`contained_intervals_in` の左端一致には既知の legacy 制約があります。緑の検証結果は、これらが修正済みであることを意味しません。

人向けのリポジトリ構成、ローカル実行、AI向け保守規則はリポジトリの `README.md` と `AGENTS.md` を参照してください。
