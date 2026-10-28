# ライセンスと競技利用

この文書はリポジトリでの実務上の案内です。法的助言ではありません。
適用範囲は [`LICENSE-SCOPE.md`](../LICENSE-SCOPE.md)、第三者由来は
[`THIRD_PARTY_NOTICES.md`](../THIRD_PARTY_NOTICES.md) を正とします。

## このリポジトリの区分

- rrrriki が作成した新規ファイルと、著作物性のある追加・改変部分は、別記がなければ Apache License 2.0 です。
- drken1215 由来の IntervalSet 本体部分は CC0 1.0 Universal のままです。Apache-2.0 は rrrriki の追加・改変にだけ適用され、元の CC0 の利用可能性を狭めません。
- NyaanNyaan/library 由来の LazyReversibleRBST 本体部分も CC0 1.0 Universal のままで、Apache-2.0 は rrrriki の追加・改変にだけ適用されます。
- EulerTour は、ユーザー申告では @rrrrikiOW が GitHub Copilot の補助を使って作成したものです。Apache-2.0 は rrrriki の著作権保護を受ける人の創作部分に適用します。この申告だけで生成断片の著作物性や第三者権利が全面的に保証されるわけではなく、外部依存の AtCoder Library は CC0 1.0 Universal のままです。
- `cpp.json` 全体は provenance が混在するため一括 Apache-2.0 の対象外です。保守対象の `@interval_set`、`@RBST`、`@EulerT` は個別の来歴と適用範囲を表示し、他の未確認エントリは対象外のままです。
- 生成文書に含まれる competitive-verifier のテンプレート資源は MIT License です。

## Apache-2.0 対象コードを配布・改変するとき

Apache-2.0 第4節に従い、少なくとも次を確認してください。

1. 受領者へ Apache License 2.0 のコピーを渡す。
2. 改変したファイルに、変更したことが分かる目立つ通知を付ける。
3. 関係する著作権・特許・商標・帰属表示を保持する。
4. 配布物に関係する [`NOTICE`](../NOTICE) の表示を、NOTICE、同梱文書、または通常帰属表示を出す画面のいずれかに含める。

`SPDX-License-Identifier` や source URL は識別と追跡に有用ですが、常に
「ライセンス本文のコピーを渡す」という要件まで単独で満たすとは限りません。
単一ソースしか提出できない環境では、提出先が何を配布するかも含めて事前に
適合方法を判断してください。

CC0 部分については帰属表示はライセンス上の必須条件ではありませんが、由来を
追跡可能にするため、drken1215 の source URL と CC0 表示を残すことを推奨します。

## コンテスト規約とは別に確認する

ライセンスに適合していても、コンテスト規約に適合するとは限りません。逆も同様です。
たとえば AtCoder では、生成 AI の利用、開始前コードの公開時刻、提出コード内の
参照 URL などに[別の公式ルール](https://info.atcoder.jp/entry/llm-rules-ja)があります。参加時点の公式ルールを必ず確認してください。

コードを読んで理解することは推奨しますが、ライセンスは学習方法を強制するための
仕組みではありません。Apache-2.0 が求めるのは、主に配布時のライセンス・変更・
帰属表示です。
