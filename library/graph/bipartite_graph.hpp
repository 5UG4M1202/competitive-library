#pragma once
#include <atcoder/dsu>
#include <vector>

// Extracted from legacy cpptemplate; historical provenance not established.
// Apache-2.0 applies only to rrrriki's copyrightable human contributions.
struct BipartiteGraph : atcoder::dsu {
  /**
   * @brief コンストラクタ
   * @tparam GraphType グラフの型
   * @param g グラフのインスタンス
   *
   * グラフを受け取り、二部グラフの判定を行います。
   * 結果は `is_bipartite` に格納されます。
   */
  template <typename GraphType>
  BipartiteGraph(const GraphType &g)
      : atcoder::dsu(g.size() * 2), color(g.size() * 2, -1), colored(false) {
    is_bipartite_flag = bipartite(g);
  }

  /**
   * @brief 二部グラフかどうかを返す関数
   * @return 二部グラフであれば true、そうでなければ false
   */
  inline bool is_bipartite() { return is_bipartite_flag; }

  /**
   * @brief 二部グラフの彩色を行う関数
   * @return 彩色が可能であれば true、そうでなければ false
   *
   * グラフが二部グラフである場合に、各連結成分に対して色を割り当てます。
   * この関数は内部で使用され、`operator[]` から呼び出されます。
   */
  bool bipartite_graph_coloring() {
    int n = color.size() / 2;
    for (int i = 0; i < n; ++i) {
      int a = leader(i);
      int b = leader(i + n);
      if (a == b) return false;
      if (color[a] == -1) {
        color[a] = 0;
        color[b] = 1;
      }
    }
    return true;
  }

  /**
   * @brief 指定した頂点の色を取得する演算子オーバーロード
   * @param i 頂点のインデックス
   * @return 頂点の色（0または1）
   *
   * 頂点の色が未割り当ての場合、彩色を行う
   */
  int operator[](int i) {
    if (!colored) {
      colored = true;
      bipartite_graph_coloring();
    }
    return color[leader(i)];
  }

 private:
  /// 各頂点の色を格納するベクター（0または1）
  std::vector<int> color;
  /// グラフが二部グラフかどうかを保持するフラグ
  bool is_bipartite_flag;
  /// 彩色済みかどうかを保持するフラグ
  bool colored;
  /**
   * @brief 二部グラフかどうかを判定する関数
   * @tparam GraphType グラフの型
   * @param g グラフのインスタンス
   * @return 二部グラフであれば true、そうでなければ false
   */
  template <typename GraphType>
  bool bipartite(const GraphType &g) {
    int n = g.size();
    for (int i = 0; i < n; ++i) {
      for (const auto &e : g[i]) {
        merge(e.from, e.to + n);
        merge(e.to, e.from + n);
      }
    }
    for (int v = 0; v < n; ++v) {
      if (same(v, v + n)) {
        return false;
      }
    }
    return true;
  }
};
