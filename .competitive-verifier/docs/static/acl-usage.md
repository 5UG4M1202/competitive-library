---
layout: page
title: ACL の代表的な機能
---

# ACL の代表的な機能

ACL が使える環境では、そのまま `#include` して使えます。外部ライブラリなので提出用コードに自動展開されません。提出先で ACL が使えるか事前に確認してください。添字は 0 始まり、区間は `[l,r)` です。

## Fenwick Tree：一点加算と区間和

一点を増減し、区間和を求めます。設定値への変更は元の値との差を加算します。演算はそれぞれ `O(log n)` です。和が型の範囲に収まるようにします。[ACL の仕様](https://atcoder.github.io/ac-library/production/document_ja/fenwicktree.html)

```cpp
#include <atcoder/fenwicktree>
#include <cassert>
int main() {
  atcoder::fenwick_tree<long long> fw(4);
  fw.add(1, 3);
  fw.add(2, 7);
  assert(fw.sum(1, 3) == 10);
  assert(fw.sum(2, 2) == 0);
}
```

## 強連結成分分解

有向辺を追加し、相互に到達できる頂点をまとめます。成分は縮約後の有向辺が前から後ろに向かう順序で返されます。[ACL の仕様](https://atcoder.github.io/ac-library/production/document_ja/scc.html)

```cpp
#include <atcoder/scc>
#include <cassert>
#include <vector>
int main() {
  atcoder::scc_graph graph(3);
  graph.add_edge(0, 1);
  graph.add_edge(1, 0);
  graph.add_edge(1, 2);
  auto groups = graph.scc();
  std::vector<int> id(3);
  for (int i = 0; i < static_cast<int>(groups.size()); ++i)
    for (int v : groups[i]) id[v] = i;
  assert(groups.size() == 2 && id[0] == id[1] && id[0] < id[2]);
}
```

## 2-SAT：二択条件の充足

`add_clause(i, f, j, g)` は `(x[i] == f) || (x[j] == g)` を追加します。解がある場合に限り `answer()` を参照します。[ACL の仕様](https://atcoder.github.io/ac-library/production/document_ja/twosat.html)

```cpp
#include <atcoder/twosat>
#include <cassert>
int main() {
  atcoder::two_sat sat(2);
  sat.add_clause(0, true, 1, true);
  sat.add_clause(0, false, 1, true);
  sat.add_clause(0, false, 1, false);
  if (!sat.satisfiable()) return 1;
  auto x = sat.answer();
  assert((x[0] || x[1]) && (!x[0] || x[1]) && (!x[0] || !x[1]));
}
```

最大流・最小費用流、文字列処理なども [ACL の機能一覧](https://atcoder.github.io/ac-library/production/document_ja/index.html) を参照してください。制約・計算量・整数型は各仕様に従います。
