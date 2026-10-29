# competitive-library

競技プログラミング用 C++ ヘッダーと、その仕様・検証結果をまとめます。実装説明は各ヘッダーの sibling Markdown にある `documentation_of` から生成されます。

## 収録内容

- `IntervalSet`: 値付き半開区間 `[l, r)` の上書き・分割・結合
- `LazyReversibleRBST`: 動的列の挿入・削除・区間集約・作用・反転
- `EulerTour`: 根付き木の LCA、辺・頂点のパス和、部分木和、重み更新
- `DistinctKSegments`: 連続区間分割の異なる値の個数の最大化
- `NQueen`: クイーン配置と探索
- `mydate`: 日付演算（負の通算月などに既知の制限あり）

## 検証方針

- 非負端点の intended correctness は、境界テストと seed 固定のランダム比較で検証します。
- 現在は `STANDALONE` テストのみです。各汎用 API 全体に自然に適合する judge-backed problem は未選定です。
- IntervalSet の負端点・一般化された `get_mex` には既知の legacy 制約があります。EulerTour の部分木一括加算と根の点更新は正しさ回帰および素朴比較で検証します。緑の検証結果や `LIBRARY_ALL_AC` のアイコンは、記載された範囲のテストが通ったことだけを示し、未検証入力まで保証するものではありません。

人向けのリポジトリ構成、ローカル実行、AI向け保守規則はリポジトリの `README.md` と `AGENTS.md` を参照してください。

## ライセンス

- [Apache License 2.0 本文]({{ '/LICENSE' | relative_url }})
- [適用範囲]({{ '/LICENSE-SCOPE.html' | relative_url }})
- [NOTICE]({{ '/NOTICE' | relative_url }})
- [第三者通知と competitive-verifier の MIT License 本文]({{ '/THIRD_PARTY_NOTICES.html' | relative_url }})

provenance が混在する `cpp.json` 全体は一括 Apache-2.0 の対象外です。IntervalSet と LazyReversibleRBST の上流 CC0 部分は CC0 のままです。EulerTour はユーザー申告の Copilot 補助による作成来歴を記録し、外部の AtCoder Library は CC0 のままです。Apache-2.0 は rrrriki の著作物性のある追加・改変にだけ適用されます。
