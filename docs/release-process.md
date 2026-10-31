# Release とスニペットの同期

この手順は公開の許可を与えません。ローカルの編集・commit、push、tag・Release の作成、公開先や設定の変更は、それぞれ依頼された範囲で実施します。

## 対象と参照URL

- 保守対象6件は `library/` の対応ヘッダーを正本とし、`cpp.json` の該当エントリだけを同期して検査します。未整理のスニペットは一括再生成しません。
- `cpp.json` の `cpptemplate` が展開する `source` は、そのスニペットを配布するReleaseの固定URL `https://github.com/5UG4M1202/competitive-library/releases/tag/<tag>` にします。旧版で生成済みのコードや、第三者の出典・ライセンスURLを一括更新しません。
- tagは既存の暦日形式（例 `v2026.10.31`、同日重複時は `-2` 等）で決め、ローカルとリモートで未使用であることを確認します。候補のURLはRelease公開前には存在しなくてもよく、公開後に実際に確認します。
- 生成ドキュメントの `blob/HEAD` は移動する参照です。Release版の固定参照と区別し、そのURLをRelease時点の内容の証拠には使いません。

## 手順と公開条件

1. Release対象の変更、tag、公開範囲を確定します。前項の `source` URLをそのtagに更新してから最終commitを作ります。authorとcommitter、未追跡ファイル、差分、来歴と通知を確認します。
2. リポジトリルートで `AGENTS.md` 記載の同期検査とローカルテストを実行し、利用可能な `competitive-verifier` による検証・文書生成、HTMLのリンク検査と画面確認を行います。失敗、未実行、既知の制約は記録し、未完了を成功と扱いません。
3. 対象commitをpushした後、`.github/workflows/verify.yml` の対象実行が**完了かつ成功**し、その `head_sha` がRelease対象commitと完全に一致することを確認します。同期検査、verify、文書生成・ビルド、リンク検査、成果物作成の失敗・中断・未実行は公開条件を満たしません。別SHAの成功は代用しません。
4. 修正によりcommitが変わった場合は、新しいSHAの検証結果を確認し直します。Actionsが一度も成功していないworkflowはドラフト扱いのままとします。
5. 成功したSHAにtagを付け、そのtagをReleaseします。Releaseに添付する `cpp.json` は**対象commitのファイルとバイト単位で一致**させ、SHA-256を計算してRelease本文と照合します。生成ドキュメント等を配布する場合、`LICENSE`、`LICENSE-SCOPE.md`、`NOTICE`、`THIRD_PARTY_NOTICES.md` を保持します。
6. 公開後、ReleaseのURL、tagが指すcommit、ActionsのSHA、`cpptemplate` の `source`、添付ファイルのdigestと本文の記載を確認します。公開後に気付いた不一致は報告し、公開済みのimmutable Releaseやtagを付け替えず、新しいtagとReleaseで訂正します。

`cpptemplate` の更新は、リポジトリ外の実際のVS Codeスニペットファイルの更新を意味しません。外部ファイルへの反映には別途明示的な依頼が必要です。
