---
title: Mo のアルゴリズム
documentation_of: ./mo.hpp
---

# Mo のアルゴリズム

先に全区間を受け取り、端点を少しずつ動かしながら順番を入れ替えて処理します。区間への要素追加・削除が速く、オンラインで答える必要のない問題に使えます。配列そのものや答えの値は保持せず、利用者が状態とコールバックを用意します。

## 例：区間内の異なる値の個数

```cpp
#include "library/misc/mo.hpp"
#include <algorithm>
#include <iostream>
#include <vector>
int main() {
  std::vector<int> a{4, 2, 4, 7};
  auto keys = a;
  std::sort(keys.begin(), keys.end());
  keys.erase(std::unique(keys.begin(), keys.end()), keys.end());
  for (auto& x : a) x = static_cast<int>(std::lower_bound(keys.begin(), keys.end(), x) - keys.begin());
  Mo mo(static_cast<int>(a.size()));
  mo.add_query(0, 4);
  mo.add_query(1, 3);
  std::vector<int> frequency(keys.size(), 0), answer(2);
  int distinct = 0;
  auto add = [&](int i) { if (frequency[a[i]]++ == 0) ++distinct; };
  auto remove = [&](int i) { if (--frequency[a[i]] == 0) --distinct; };
  mo.run(add, remove, [&](int id) { answer[id] = distinct; });
  for (int result : answer) std::cout << result << '\n';
}
```

出力は `3`、`2` です。値を座標圧縮し、出現数が 0 から 1 に変わるときに種類数を増やします。検証ページの「提出用の利用例」は本体と `main()` をまとめてコピーでき、外部ライブラリは不要です。問題に合わせて配列とクエリの入出力を変更してください。

`Mo(n)` の `n` は非負。`add_query(l,r)` は `0 <= l <= r <= n` の半開区間を登録し、0 からの登録順 ID を返します。空区間・クエリなし・`n=0` も扱えます。登録数は `INT_MAX` 未満で、クエリと並べ替え用配列を確保できる必要があります。範囲外の入力は `assert` で検出しますが、`NDEBUG` 時も前提を守ってください。

`run(add_left, add_right, remove_left, remove_right, answer)` は、空の `[0,0)` に対応する状態から開始します。追加・削除の各関数に渡すのは、実際に追加・削除する要素の添字です。左右を区別しない問題では例のように同じ関数を渡せます。`answer(id)` はその区間の状態が整ったときに呼ばれ、回答順は登録順とは限りません。呼び出し後の状態は最後の処理区間に対応します。再実行する場合は外部状態を空に戻してください。

左右で同じ操作なら `run(add, remove, answer)` を使えます。3 引数版は関数オブジェクトをコピーせずに呼び出します。5 引数版は各引数を値で受け取るため、コピーできないオブジェクトや呼び出し回数などを外側のオブジェクトへ記録したい場合は `std::ref(callback)` を渡してください。状態はどちらも利用者が用意し、`answer` に返すのは登録順 ID です。

順序に依存する例は区間内の転倒数です。左に値 x を追加すると現在区間で x より小さい値の個数が増加分、右に追加すると x より大きい値の個数が増加分です。削除はその逆であり、左右別の 5 引数版を使います。各操作で使用する頻度配列や Fenwick Tree の計算量は別途かかります。配列の内容がクエリの間で変わる「更新付き Mo」には、この API は対応しません。

`q > 0` のとき、既定では幅 `B = max(1, floor(n/sqrt(q)))` で左端を分割し、同じブロックの右端を交互に昇順・降順にします。`set_block_size(B)` に正の値を渡すと幅を指定でき、`0` で自動設定に戻せます。幅によって移動回数が変わるため、変更するときは実際の入力で測定してください。負の幅は不可です。整列は `O(q log q)`、端点移動は `O(qB + n(1+n/B))` 回、補助メモリは `O(q)` です。端点操作と回答のコストは別途掛かります。結果がどの処理順でも正しくなるよう、状態操作を実装してください。

従来の `@mos` の区間走査を基に独立 API を整備しました。具体的な来歴は未確認です。`@mos` 側は 1-indexed 閉区間入力を 0-indexed 半開区間に変換する使用例として維持し、`N`、`Q`、`ll` と利用者による追加・削除処理が必要です。以前は右端を余分に減らし、クエリ 0 件のブロック幅も不正だったため修正しました。

ローカルテストは空区間・空クエリ・ブロック幅の変更に加え、左右の追加・削除順序と転倒数を素朴計算で照合します。`@mos` の展開にも区間和の処理を入れて別途検証しています。
