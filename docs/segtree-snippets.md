# セグメント木スニペット

`cpp.json` のセグメント木スニペットは、競技用テンプレートのグローバル定義に展開する使用例です。`<atcoder/all>`、`using namespace atcoder`、`ll` を含む `cpptemplate` を想定します。同じ `S`、`op` などを定義するため、同じ翻訳単位にそのまま同時展開できません。目的に合う1つを選びます。`@segtree_dynamic` はさらに `cpptemplate` 内の `dynamic_lazy_segtree` を使います。

| prefix | 操作 | `main` 内での構築 |
| --- | --- | --- |
| `@segtree_normal` | 点代入・区間和 | `Segtree seg(N);` |
| `@segtree_lazy` | 区間加算・区間和 | `LazySegtree seg(vector<S>(N, {0, 1}));` |
| `@segtree_lazy_min` | 区間加算・区間最小値 | `LazySegtree seg(vector<S>(N, {0, 1}));` |
| `@segtree_lazy_max` | 区間加算・区間最大値 | `LazySegtree seg(vector<S>(N, {0, 1}));` |
| `@segtree_dynamic` | 広い整数座標の区間代入・区間和 | `DynamicSegtree seg(0, N, init);` |

定義をグローバル側に展開し、`N` の入力後に `main` 内で木を構築します。範囲は `[l, r)` です。遅延版と動的版の区間和は `seg.prod(l, r).value` から取り出します。遅延版の各要素は `{初期値, 1}` が必要で、`LazySegtree seg(N)` のままでは各要素の長さが0になります。

## 区間加算と最小値・最大値

例えば最小値なら `@segtree_lazy_min` を `main` の外で展開し、次のように使います。最大値では `@segtree_lazy_max` を代わりに展開します。

```cpp
vector<S> a{{5, 1}, {2, 1}, {8, 1}};
LazySegtree seg(a);
seg.apply(0, 2, 3);              // {8, 5, 8}
ll minimum = seg.prod(0, 3).value; // 5
S empty = seg.prod(1, 1);         // empty.size == 0
```

一律加算では最小値・最大値も同じ値だけ変わるので、`mapping` は `value += f` です。区間和のような `f * size` は不要です。`size` は空区間を識別し、`op` と `mapping` が空区間をそのまま扱うために残しています。空区間の `value` は 0 ですが、最小値や最大値を意味しないため `size == 0` で判定してください。これにより通常値が `LLONG_MIN` や `LLONG_MAX` でも番兵と衝突しません。加算後の値・作用の合計・区間の長さは `ll` に収まる必要があります。

これらは**区間加算**用です。区間代入や一次変換には作用の定義変更が必要です。区間代入と最小値・最大値も含めた型は [Monoid と ACL セグメント木](../library/data_structure/monoid_segtree.md) にあります。

動的版の初期配列はすべて1で、`N > 0` が必要です。`F` は `optional<ll>` で、数値の代入と `nullopt` による無操作を区別します。サイズも `ll` で持ちます。代入する値、長さ、積、和と、作用の合成などの中間値が `ll` に収まることが必要です。各データ構造や Monoid の選択・制約は [動的遅延セグメント木](../library/data_structure/dynamic_lazy_segtree.md) と [Monoid と ACL セグメント木](../library/data_structure/monoid_segtree.md) を参照してください。

従来の `seg()` は関数宣言で、期待した変数として使えなかったため、各型の alias と `main` 内での構築例に変更しました。従来の遅延版は和と区間加算の定義が両立していなかったため、要素数を保持する区間加算・区間和へ変更しています。動的版は旧来の番兵値との衝突と `int` のサイズ制限をなくしています。コードを既に展開済みのファイルには、この変更は反映されません。

操作定義は [betrue12 さんの ACL Lazy Segtree チートシート](https://betrue12.hateblo.jp/entry/2020/09/23/005940) を参照しており、元の利用条件は [第三者通知](../THIRD_PARTY_NOTICES.md) に記録しています。`tools/test_segtree_examples.py` は各例を実際に展開・コンパイルし、更新・集約と大きな座標を検査します。実行には ACL の include path が必要です。
