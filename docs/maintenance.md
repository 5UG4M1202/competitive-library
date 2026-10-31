# Maintenance policy

## Snippet synchronization

`@mydate`、`@DistinctMax`、`@NQUEEN` はそれぞれのヘッダーから同期し、`python3 tools/check_unresolved_snippet_sync.py` で定義の一致と展開テストを確認します。ACL の include path が必要です。

保守対象として対応付けたコンポーネントでは、`library/` のヘッダーを正本とし、`cpp.json` の対応エントリを現在利用できる snippet として同期します。対応は `interval_set.hpp` と `@interval_set`、`lazy_reversible_rbst.hpp` と `@RBST`、`euler_tour.hpp` と `@EulerT`、`mydate.hpp` と `@mydate`、`distinct_k_segments.hpp` と `@DistinctMax`、`nqueen.hpp` と `@NQUEEN` です。

- コード、テスト、コンポーネント文書を先に更新し、対応エントリだけを同期する。
- 未整理のエントリ、`prefix`、`description`、関係のない本文と整形は変更しない。
- 出典とライセンス表示を snippet 本文にも残し、混在する権利関係を一括で再許諾しない。
- `cpp.json` は VS Code の JSONC として読み取れる状態を維持する。先頭コメントや既存の整形を JSON への再シリアライズで失わない。
- `python3 tools/check_interval_set_snippet_sync.py` で JSON 構造、対応関数、出典表示、代表テンプレートでのコンパイルと回帰を確認する。
- `python3 tools/check_rbst_snippet_sync.py` で RBST の対応定義、出典表示、代表テンプレートでのコンパイルと回帰を確認する。
- `python3 tools/check_euler_tour_snippet_sync.py` で EulerTour の全定義、来歴表示、`Graph` と AtCoder Library を含む代表テンプレートでのコンパイルと実行を確認する。実行には ACL の include path が必要。
- リポジトリ外の live VS Code snippet は別管理とし、明示された依頼なしに更新しない。

ヘッダーを snippet から逆生成したり、対応付けていないエントリを一括再生成したりしません。

## Extracting a snippet

1. snippet 本文と隠れた依存（型 alias、macro、using、他 snippet）を読む。
2. 既存 API と意味を保って `library/<category>/name.hpp` へ抽出する。
3. standalone include に必要な標準ヘッダーと名前修飾だけを補う。
4. sibling Markdown に境界、前提、複雑度、既知の legacy 制約を書く。
5. boundary test と決定的 randomized test を追加する。
6. 抽出中に見つけた bug は、互換抽出と同じ変更で黙って直さない。再現例を残し、別変更として扱う。

## Verification

`STANDALONE` は exit code 0 を成功とする local test に使います。`LOCALCASE` は入出力 fixture が本質的な場合、`PROBLEM` は Library Checker、AOJ、yukicoder に実際に適合する問題がある場合だけ使います。Markdown は `documentation_of` でヘッダーに関連付けます。

## Provenance and licensing

新しいコードや第三者由来の実装を追加する前に、source URL、確認した版、適用ライセンス、ローカルでの変更内容を記録します。適用範囲は [`../LICENSE-SCOPE.md`](../LICENSE-SCOPE.md)、第三者通知は [`../THIRD_PARTY_NOTICES.md`](../THIRD_PARTY_NOTICES.md)、競技提出時の実務案内は [`license-and-contest-use.md`](license-and-contest-use.md) を参照してください。

`cpp.json` は provenance が混在しています。保守対象の `@interval_set` と `@RBST` は、上流 CC0 部分と rrrriki の Apache-2.0 対象の追加・変更を区別して表示します。`@EulerT` はユーザー申告の来歴（@rrrrikiOW が GitHub Copilot の補助を使って作成）を保持し、具体的な第三者ソースが示されていないことと、Copilot 利用だけでは権利関係の全面保証にならないことを明記します。それ以外の未確認エントリは一括 Apache-2.0 の対象外であり、出典表示がないことをオリジナルまたは自由利用可能である証拠として扱いません。生成文書を配布する場合は、生成に取り込まれるテンプレートの通知を含め、root のライセンス・scope・NOTICE・第三者通知を成果物へ残します。

## Release and publication

ローカルの整理、commit、push、PR、Release、GitHub Pages、repository 設定変更は別の権限です。依頼された範囲を越えて公開しません。Release を作る場合は、対象 commit、tag、添付 digest、公開範囲を事前に確認します。

スニペットの参照URL、検証する commit、Actions の成功条件、Release と添付ファイルの対応は [`release-process.md`](release-process.md) に従います。
