# Verification

## Layout

- `local/`: `competitive-verifier: STANDALONE` の決定的ユニットテスト。
- `aoj/`, `yosupo/`: 実際に適合する問題が見つかった時だけ追加する judge-backed test。空ディレクトリは作りません。

IntervalSet に自然に対応する公開問題は今回選定していません。無関係な問題を `PROBLEM` に指定するより、境界テストと単純実装とのランダム比較を維持します。

## Local setup

必要条件は Python 3.10 以上、C++20 コンパイラ、`competitive-verifier` です。公式の導入元は [competitive-verifier](https://github.com/competitive-verifier/competitive-verifier) で、PyPI package 名も `competitive-verifier` です。CIと次の例は、確認済みの `4.2.2` に固定しています。依存を導入する場合は、system Python ではなく専用 virtual environment を利用してください。

```sh
python3 -m venv /tmp/competitive-library-verifier-venv
/tmp/competitive-library-verifier-venv/bin/python -m pip install competitive-verifier==4.2.2
PATH="/tmp/competitive-library-verifier-venv/bin:$PATH" bash tools/run_verifier.sh
```

`tools/run_verifier.sh` は `oj-resolve`、`verify --check-error`、`docs` を順番に実行し、既定では `verify-files.json`、`verify-result.json`、Jekyll source、bundle・cache 用の `runtime/` をOSの一時ディレクトリへ出力します。第一引数に出力ディレクトリを渡すこともできます。生成物はリポジトリへ追加しません。完成HTMLのローカルbuildには別途BundlerとJekyllが必要です。

簡易ローカル実行は `tools/run_local_tests.ps1` を利用できます。Visual Studio の場合は Developer PowerShell で実行するか、`-Compiler cl` を指定してください。

## Documentation

`library/data_structure/interval_set.md` の `documentation_of` は `interval_set.hpp` を指します。`.competitive-verifier/docs/index.md` が生成サイトのトップページ、`_config.yml` が日本語タイトル情報です。テストファイルは `DISPLAY no-index` により検証対象のまま索引から除外します。Doxygen は使用しません。

## CI design

`.github/workflows/verify.yml` は pull request、`main` push、手動実行で検証、文書生成、Jekyll buildを行います。公式 `verify` Action は既定の `check: false` では失敗を job failure にしないため、`check: true` を明示しています。

- pull request と手動実行: 生成サイトを通常の Actions artifact として保存します。Pages には公開しませんが、公開リポジトリの artifact は機密境界ではなく、リポジトリを閲覧できる利用者がダウンロードできる場合があります。生成文書には秘密情報、個人情報、ローカルの個人パスを含めないでください。
- `main` push: 将来の Pages deployment が利用できる `github-pages` artifact を作りますが、deploy job はまだありません。

Pages を実際に公開する時は、別途明示的な許可を得た後に、repository の Pages source を GitHub Actions に設定し、`actions/configure-pages@v6`、`pages: write` と `id-token: write` を持つ `actions/deploy-pages@v5` job、および `github-pages` environment を追加します。

2026-10-27 に、E drive 上の隔離 venv にある `competitive-verifier` 4.2.2 を使い、CLI の resolve、2本の `STANDALONE` verify、Jekyll source 生成までローカルで確認しました。この venv は repository の一部ではなく、他の環境では上記 setup が別途必要です。完成HTMLの build と GitHub Actions は未実行です。
