# セグメント木スニペット

`cpp.json` の3項目は、競技用テンプレートのグローバル定義に展開する使用例です。`<atcoder/all>`、`using namespace atcoder`、`ll` を含む `cpptemplate` を想定します。同じ `S`、`op` などを定義するため、3項目を同じ翻訳単位にそのまま同時展開できません。目的に合う1つを選びます。`@segtree_dynamic` はさらに `cpptemplate` 内の `dynamic_lazy_segtree` を使います。

| prefix | 操作 | `main` 内での構築 |
| --- | --- | --- |
| `@segtree_normal` | 点代入・区間和 | `Segtree seg(N);` |
| `@segtree_lazy` | 区間加算・区間和 | `LazySegtree seg(vector<S>(N, {0, 1}));` |
| `@segtree_dynamic` | 広い整数座標の区間代入・区間和 | `DynamicSegtree seg(0, N, init);` |

定義をグローバル側に展開し、`N` の入力後に `main` 内で木を構築します。範囲は `[l, r)` です。遅延版と動的版の区間和は `seg.prod(l, r).value` から取り出します。遅延版の各要素は `{初期値, 1}` が必要で、`LazySegtree seg(N)` のままでは各要素の長さが0になります。

動的版の初期配列はすべて1で、`N > 0` が必要です。`F` は `optional<ll>` で、数値の代入と `nullopt` による無操作を区別します。サイズも `ll` で持ちます。代入する値、長さ、積、和と、作用の合成などの中間値が `ll` に収まることが必要です。各データ構造や Monoid の選択・制約は [動的遅延セグメント木](../library/data_structure/dynamic_lazy_segtree.md) と [Monoid と ACL セグメント木](../library/data_structure/monoid_segtree.md) を参照してください。

従来の `seg()` は関数宣言で、期待した変数として使えなかったため、各型の alias と `main` 内での構築例に変更しました。従来の遅延版は和と区間加算の定義が両立していなかったため、要素数を保持する区間加算・区間和へ変更しています。動的版は旧来の番兵値との衝突と `int` のサイズ制限をなくしています。コードを既に展開済みのファイルには、この変更は反映されません。

操作定義は [betrue12 さんの ACL Lazy Segtree チートシート](https://betrue12.hateblo.jp/entry/2020/09/23/005940) を参照しており、元の利用条件は [第三者通知](../THIRD_PARTY_NOTICES.md) に記録しています。`tools/test_segtree_examples.py` は3例を実際に展開・コンパイルし、更新・集約と大きな座標を検査します。実行には ACL の include path が必要です。
