# Maintenance policy

## Legacy snapshot

`cpp.json` は既存利用者向けの固定 snapshot です。個別ヘッダーの更新を機械的に書き戻しません。変更依頼が明示されない限り、byte-for-byte で保持します。`tools/check_legacy_snapshot.ps1` で既知の SHA-256 を確認できます。

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

`cpp.json` は provenance が完全でないため、一括 Apache-2.0 の対象外です。出典表示がないことを、オリジナルまたは自由利用可能である証拠として扱いません。生成文書を配布する場合は、生成に取り込まれるテンプレートの通知を含め、root のライセンス・scope・NOTICE・第三者通知を成果物へ残します。

## Release and publication

ローカルの整理、commit、push、PR、Release、GitHub Pages、repository 設定変更は別の権限です。依頼された範囲を越えて公開しません。Release を作る場合は、対象 commit、tag、添付 digest、公開範囲を事前に確認します。
