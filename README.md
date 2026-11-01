# competitive-library

競技プログラミング向け C++ ライブラリを、実装・説明・検証を近接させて保守するためのリポジトリです。

## 導線

- [`docs/library-guide.md`](docs/library-guide.md): 操作や用途から利用する部品を探す。
- [`docs/snippet-examples.md`](docs/snippet-examples.md): 二分探索・順列・格子方向など使用例の前提と依存関係。
- [`docs/template-usage.md`](docs/template-usage.md): 提出用テンプレートの入出力・マクロ・依存関係。
- [`docs/header-scope-review.md`](docs/header-scope-review.md): 独立ヘッダーと snippet の判断理由。
- [`library/`](library/README.md): 提出コードから利用するヘッダー。
- [`verify/`](verify/README.md): local / judge-backed 検証。
- [`docs/maintenance.md`](docs/maintenance.md): 抽出、互換性、Release の運用方針。
- [`docs/commit-conventions.md`](docs/commit-conventions.md): コミットメッセージ、変更分類、分割単位の規約。
- [`docs/release-process.md`](docs/release-process.md): スニペットの参照URLとRelease・Actionsの同期手順。
- [`docs/license-and-contest-use.md`](docs/license-and-contest-use.md): ライセンスの適用範囲、配布時の注意、コンテスト規約との区別。
- [`LICENSE`](LICENSE) / [`LICENSE-SCOPE.md`](LICENSE-SCOPE.md) / [`THIRD_PARTY_NOTICES.md`](THIRD_PARTY_NOTICES.md): Apache-2.0 本文、適用範囲、第三者由来。
- [`cpp.json`](cpp.json): 現行の VS Code snippet 集。保守対象として対応付けた項目は `library/` のヘッダーから同期し、未整理の項目は個別に扱います。

## AI / Codex で作業する

最初に [`AGENTS.md`](AGENTS.md) で保守規則と実行コマンドを確認し、依頼の主目的に合うスキルだけを選びます。すべてのスキルを順番に実行する必要はありません。

- [`$cp-library-add`](.agents/skills/cp-library-add/SKILL.md): 独立した新規部品を追加する。例: 「Sparse Table を文書とテスト付きで追加して」
- [`$cp-library-extend`](.agents/skills/cp-library-extend/SKILL.md): 既存 API を互換性に注意して拡張する。例: 「IntervalSet に `split_at` を追加して」
- [`$cp-library-verify`](.agents/skills/cp-library-verify/SKILL.md): テストや competitive-verifier 連携を整える。例: 「このヘッダーに境界テストを追加し、適切な judge 問題があるか調べて」
- [`$cp-library-review`](.agents/skills/cp-library-review/SKILL.md): 既定では修正せず、バグ・契約・計算量をレビューする。例: 「IntervalSet の端点処理を反例付きでレビューして」
- [`$cp-library-release-check`](.agents/skills/cp-library-release-check/SKILL.md): commit、tag、成果物、公開範囲を読み取り専用で事前確認する。例: 「vNext の公開準備を確認して。公開はしないで」

## 現在の vertical slice

- [Monoid と ACL セグメント木](library/data_structure/monoid_segtree.md): 区間集約と作用を型として定義し、ACL で利用。
- [動的遅延セグメント木](library/data_structure/dynamic_lazy_segtree.md): 広い座標の区間更新・集約・境界探索。来歴は未確認。
- [UnionFindWithPotential](library/data_structure/union_find_with_potential.md): 頂点間の重みの差と制約の整合性。
- [`IntervalSet`](library/data_structure/interval_set.md): 値付き半開区間 map。
- [`LazyReversibleRBST`](library/data_structure/lazy_reversible_rbst.md): 列の挿入・削除・区間作用・反転を扱う乱択平衡二分木。
- [`EulerTour`](library/graph/euler_tour.md): 根付き木の LCA、パス和、部分木和、重み更新。
- [`DistinctKSegments`](library/data_structure/distinct_k_segments.md): 連続区間分割における異なる値の個数の最大化。
- [`NQueen`](library/data_structure/nqueen.md): Nクイーンの配置と探索。
- [`mydate`](library/misc/mydate.md): 日付の比較・演算。

Union-Find は独自実装を増やさず、通常は AtCoder Library の `atcoder::dsu` を利用する方針です。

## ローカル検証

PowerShell と C++20 コンパイラが利用できる環境では次を実行します。

```powershell
python3 tools/check_interval_set_snippet_sync.py
python3 tools/check_rbst_snippet_sync.py
python3 tools/check_euler_tour_snippet_sync.py  # AtCoder Library の include path が必要
python3 tools/check_unresolved_snippet_sync.py  # AtCoder Library の include path が必要
powershell -ExecutionPolicy Bypass -File tools/run_local_tests.ps1
```

`competitive-verifier` の導入・実行と CI の状態は [`verify/README.md`](verify/README.md) を参照してください。依存ソフトのインストール、GitHub Pages、repository 設定変更には明示的な許可が必要です。
