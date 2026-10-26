# Verification

## Layout

- `local/`: `competitive-verifier: STANDALONE` の決定的ユニットテスト。
- `aoj/`, `yosupo/`: 実際に適合する問題が見つかった時だけ追加する judge-backed test。空ディレクトリは作りません。

IntervalSet に自然に対応する公開問題は今回選定していません。無関係な問題を `PROBLEM` に指定するより、境界テストと単純実装とのランダム比較を維持します。

## Local setup

必要条件は Python 3.9 以上、C++20 コンパイラ、`competitive-verifier` です。次は利用者が明示的に環境を整える場合の例であり、この scaffolding 作業ではインストールしていません。

```sh
python -m pip install competitive-verifier
competitive-verifier oj-resolve --include library verify --config .competitive-verifier/config.toml > verify-files.json
COMPETITIVE_VERIFY_FILES_PATH=verify-files.json competitive-verifier verify --check-error
```

PowerShell では最後の行の代わりに `$env:COMPETITIVE_VERIFY_FILES_PATH = "verify-files.json"` を設定してから `competitive-verifier verify --check-error` を実行します。

簡易ローカル実行は `tools/run_local_tests.ps1` を利用できます。Visual Studio の場合は Developer PowerShell で実行するか、`-Compiler cl` を指定してください。

## CI draft

`.github/workflows/verify.yml` は checkout、setup、`oj-resolve`、verify の最小構成です。公式 `verify` Action は既定の `check: false` では失敗を job failure にしないため、このリポジトリでは `check: true` を明示しています。Pages の生成・公開は含みません。

CI はまだ push されておらず、実行もされていません。
