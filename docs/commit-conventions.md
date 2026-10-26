# コミット規約

この文書を、このリポジトリにおけるコミットメッセージと変更分類の唯一の基準とします。この規約は stage、commit、push、Release、`cpp.json` の変更を許可するものではありません。それぞれ別途明示された権限が必要です。

## 形式

```text
type(scope): 日本語の要約
```

- `type` と `scope` は小文字の英語、要約は簡潔な日本語にします。要約末尾に句点は付けません。固定の文字数制限は設けません。
- `scope` は通常付けます。リポジトリ全体に及ぶ変更で適切な単一 scope がない場合だけ省略できます。Conventional Commits 本体では scope は任意ですが、このリポジトリでは追跡しやすさのため通常必須とします。
- type は拡張子や変更ファイルの種類ではなく、変更の主目的で選びます。該当する具体的な type がある場合は `chore` を使いません。

## Type

| Type | 用途 |
| --- | --- |
| `feat` | 新しい部品、公開 API、利用者向け機能 |
| `fix` | 誤った挙動や指示の修正 |
| `perf` | 意味を保つ性能改善 |
| `refactor` | 意味を保つ内部構造の変更 |
| `test` | テストだけの追加・変更 |
| `docs` | 説明文だけの変更 |
| `ci` | CI 設定や CI 上の検証手順 |
| `build` | ビルド方法、依存関係、配布生成 |
| `style` | 挙動に影響しない書式変更 |
| `chore` | 上記に当てはまらない保守作業 |
| `revert` | 既存コミットの取り消し |

## Scope

scope は安定した機能名を優先し、網羅的な許可リストにはしません。

- 部品と、その部品専用の文書・テスト: `interval-set`, `rolling-hash` など
- 共通の検証基盤: `verify`
- repository-local skill: `skills`
- Codex のリポジトリ規則: `agents`
- リポジトリ全体の構成: `repo`
- Release 準備や成果物: `release`

## 分類

- 独立した新規部品や公開 API の追加は `feat(<component>)` とします。
- 対応する挙動を保った純粋な移動・分割は `refactor(<component>)` とします。
- bug fix、回帰テスト、その修正に必要な説明更新は、一つの `fix(<component>)` にまとめます。
- テストだけの変更は `test`、CI 設定は `ci` とします。
- skill の振る舞いや能力の追加は `feat(skills)`、誤った手順の修正は `fix(skills)`、説明文だけなら `docs(skills)` とします。
- 一つのコミットは、一度のレビューまたはrevertで理解できる一つの目的にします。ファイルごとには分割せず、無関係な整形や別機能を混ぜません。

## 本文、破壊的変更、revert

理由、変更前後、互換性、検証結果が要約だけでは伝わらない場合は本文に記載します。buildとtestは原則として検証可能な状態を保ち、既存の known bug、characterization test、未実行の確認は隠さず明記します。

互換性のない API・保証の変更には `!` と `BREAKING CHANGE:` footer を使用し、移行方法を書きます。文書化済みの計算量保証を悪化させる変更も破壊的変更になり得ます。

```text
feat(interval-set)!: 端点型の要件を変更

BREAKING CHANGE: 利用側の端点型を64bit符号付き整数へ変更する
```

`revert` の本文には取り消す元コミットの hash を記載します。

```text
revert(interval-set): 区間分割APIの追加を取り消す

Reverts: 0123456789abcdef
```

## 例

```text
feat(interval-set): 区間分割APIを追加
fix(interval-set): 左端一致区間の包含数を正しく数える
test(interval-set): 最大端点の境界テストを追加
refactor(rolling-hash): 公開APIを保ったまま内部実装を分離
perf(rolling-hash): ハッシュ照合の一時確保を削減
ci(verify): competitive-verifierの失敗をCIへ反映
feat(skills): ライブラリ追加手順を拡張
fix(skills): 公開前確認の誤った権限説明を修正
docs(skills): コミット規約への導線を追加
```

## 安全確認

- stage 前に対象を明示し、`git add -A` で未確認の変更を一括追加しません。
- commit 前に staged diff を確認し、意図した一単位だけが含まれることを確かめます。
- commit 本文へ secret、token、資格情報、不要な個人情報を書きません。
- 公開前に author と committer の両方が意図した no-reply identity であることを確認します。

この分類から SemVer の更新やRelease作成は自動的には導きません。現在のcalendar-style tagとimmutable Releaseの運用は変更しません。

参考: [Conventional Commits 1.0.0](https://www.conventionalcommits.org/en/v1.0.0/)、[AngularJS commit message guidelines](https://github.com/angular/angular.js/blob/master/DEVELOPERS.md#type)。日本語要約、scopeを通常付けること、目的単位の分割はこのリポジトリ固有の選択です。
