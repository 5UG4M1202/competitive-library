# IntervalSet local tests

## Intended-correctness coverage

- 非負端点での上書き、分割、同値区間の結合、削除。
- 点・1 区間被覆、値取得、被覆長、callback 差分。
- 単一値の被覆集合としての `get_mex`。
- 単純配列モデルとの seed 固定 5,000 operation 比較。

## Characterization-only coverage

次は元 snippet の既知挙動を再現するだけで、正しさを保証するテストではありません。

| Case | Intended result | Current result |
| --- | --- | --- |
| `[2,3)`, `[3,6)`, `[6,8)` に `contained_intervals_in(2,8)` | `3` | `2` |
| `[0,1):1`, `[1,2):2` に `get_mex(0)` | `2` | `1` |
| `update(-10,-5,1)` の後に `update(-10,-7,2)` | 2 個の非空区間 | 空区間を含む 3 node |

intended result を assertion する regression は、faithful extraction と bug fix を分離するため今回は無効です。修正する変更では、これらを failing test として先に有効化してください。
