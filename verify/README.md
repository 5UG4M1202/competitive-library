# Verification

## Layout

- `local/`: `competitive-verifier: STANDALONE` の決定的ユニットテスト。
- `aoj/`, `yosupo/`: 実際に適合する問題が見つかった時だけ追加する judge-backed test。空ディレクトリは作りません。

IntervalSet、LazyReversibleRBST、EulerTour の汎用 API 全体に自然に対応する公開問題は今回選定していません。無関係な問題を `PROBLEM` に指定するより、境界テストと単純実装とのランダム比較を維持します。

## Local setup

必要条件は Python 3.10 以上、C++20 コンパイラ、`competitive-verifier` です。公式の導入元は [competitive-verifier](https://github.com/competitive-verifier/competitive-verifier) で、PyPI package 名も `competitive-verifier` です。CIと次の例は、確認済みの `4.2.2` に固定しています。依存を導入する場合は、system Python ではなく専用 virtual environment を利用してください。

```sh
python3 -m venv /tmp/competitive-library-verifier-venv
/tmp/competitive-library-verifier-venv/bin/python -m pip install competitive-verifier==4.2.2
PATH="/tmp/competitive-library-verifier-venv/bin:$PATH" bash tools/run_verifier.sh
```

EulerTour の検証には AtCoder Library が必要です。既存の ACL checkout を使う場合は、その親ディレクトリ（直下に `atcoder/` がある場所）を指定します。

```sh
export ACL_INCLUDE_PATH=/path/to/ac-library
export CPLUS_INCLUDE_PATH="$ACL_INCLUDE_PATH${CPLUS_INCLUDE_PATH:+:$CPLUS_INCLUDE_PATH}"
PATH="/tmp/competitive-library-verifier-venv/bin:$PATH" bash tools/run_verifier.sh
```

`tools/run_verifier.sh` は `oj-resolve`、`verify --check-error`、`docs` を順番に実行し、既定では `verify-files.json`、`verify-result.json`、Jekyll source、bundle・cache 用の `runtime/` をOSの一時ディレクトリへ出力します。第一引数に出力ディレクトリを渡すこともできます。生成物はリポジトリへ追加しません。生成した Jekyll source には root の `LICENSE`、`LICENSE-SCOPE.md`、`NOTICE`、`THIRD_PARTY_NOTICES.md` もコピーし、project と生成テンプレートの通知を保持します。完成HTMLのローカルbuildには別途BundlerとJekyllが必要です。

簡易ローカル実行は `tools/run_local_tests.ps1` を利用できます。Visual Studio の場合は Developer PowerShell で実行するか、`-Compiler cl` を指定してください。

`tools/run_verifier.sh` は verifier の resolve より前に4本の同期検査を実行します。これらは `cpp.json` を JSONC として読み、保守対象6件が各ヘッダーの対応実装・来歴表示と一致することを確認し、展開したコードをコンパイルして決定的な回帰を実行します。EulerTour と DistinctKSegments の同期検査には上記の ACL include path が必要です。

## Documentation

生成 HTML をブラウザで確認する手順は [ローカルプレビュー](../docs/preview.md) を参照してください。

HTML を生成した後、`python3 tools/check_generated_html.py <生成HTMLディレクトリ> <baseurl>` で内部リンクとアセットの存在を確認できます。`baseurl` が空なら第2引数は省略します。外部リンクの到達性、ページ内アンカー、ブラウザ表示は別途確認します。

生成サイトでは verifier の `docs/static` に置いたテンプレートで、ライブラリ・テストの出力先に `.html` を付け、ライセンスと元ソースへのリンクを調整しています。狭い画面では長いパスを折り返し、コードブロック内を横スクロールできます。GitHub Actions の結果はテキストリンクから確認します。GitHub Actions 自体は未確認です。

追加された `mydate`、`DistinctKSegments`、`NQueen` もそれぞれ `STANDALONE` テストで検証します。`tools/check_unresolved_snippet_sync.py` は対応する3スニペットとヘッダーの一致、およびスニペット展開時のテストを確認します。`DistinctKSegments` の検証には ACL が必要です。

各 sibling Markdown の `documentation_of` は対応ヘッダーを指します。EulerTour は `library/graph/euler_tour.md` から `euler_tour.hpp` へ関連付けます。`.competitive-verifier/docs/index.md` が生成サイトのトップページ、`_config.yml` が日本語タイトル情報です。テストファイルは `DISPLAY no-index` により検証対象のまま索引から除外します。Doxygen は使用しません。

## CI design

`.github/workflows/verify.yml` は pull request、`main` push、手動実行で検証、文書生成、Jekyll buildを行います。公式 `verify` Action は既定の `check: false` では失敗を job failure にしないため、`check: true` を明示しています。

- pull request と手動実行: 生成サイトを通常の Actions artifact として保存します。Pages には公開しませんが、公開リポジトリの artifact は機密境界ではなく、リポジトリを閲覧できる利用者がダウンロードできる場合があります。生成文書には秘密情報、個人情報、ローカルの個人パスを含めないでください。
- `main` push: 将来の Pages deployment が利用できる `github-pages` artifact を作りますが、deploy job はまだありません。

Pages を実際に公開する時は、別途明示的な許可を得た後に、repository の Pages source を GitHub Actions に設定し、`actions/configure-pages@v6`、`pages: write` と `id-token: write` を持つ `actions/deploy-pages@v5` job、および `github-pages` environment を追加します。

2026-10-29 に、E drive 上の隔離 venv にある `competitive-verifier` 4.2.2 を使い、CLI の resolve、9本の `STANDALONE` verify、Jekyll source 生成までローカルで確認しました。この venv は repository の一部ではなく、他の環境では上記 setup が別途必要です。GitHub Actions は未実行です。
