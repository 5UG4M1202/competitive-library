#pragma once
#include <cstddef>
#include <iostream>
#include <vector>

// Closely matches ei1333/library graph-template.hpp; historical source unconfirmed.
// Comparison: https://github.com/ei1333/library/blob/088b4c8b49f85194099305d998e7113dc3d5dd2e/graph/graph-template.hpp
// Comparison source license: Unlicense. See THIRD_PARTY_NOTICES.md.
template <typename T = int>
struct Edge {
  int from, to;  // エッジの始点と終点
  T cost;        // エッジの重み
  int idx;       // エッジのインデックス（オプション）

  // デフォルトコンストラクタ
  Edge() = default;

  // エッジをコストに基づいて比較するための演算子オーバーロード
  bool operator<(const Edge &other) const { return cost < other.cost; }
  bool operator>(const Edge &other) const { return cost > other.cost; }
  friend std::ostream &operator<<(std::ostream &os, const Edge &edge) {
    os << edge.to;
    return os;
  }

  // コンストラクタ
  Edge(int from, int to, T cost = 1, int idx = -1)
      : from(from), to(to), cost(cost), idx(idx) {}

  // エッジの終点をintとして取得するためのキャスト演算子
  operator int() const { return to; }
};

/**
 * @brief Graphクラスはグラフのデータ構造を表します。
 * @tparam T エッジの重みの型（デフォルトはint）
 */
template <typename T = int>
struct Graph {
  std::vector<std::vector<Edge<T> > > g;  // 各ノードから出ているエッジのリスト
  int es = 0;                   // エッジの数

  // デフォルトコンストラクタ
  Graph() = default;

  // ノード数nを指定するコンストラクタ
  explicit Graph(int n) : g(n), es(0) {}

  // グラフのサイズ（ノードの数）を返す
  std::size_t size() const { return g.size(); }

  // 有向エッジを追加する関数
  void add_directed_edge(int from, int to, T cost = 1) {
    g[from].emplace_back(from, to, cost, es++);
  }

  // 無向エッジを追加する関数
  void add_edge(int from, int to, T cost = 1) {
    g[from].emplace_back(from, to, cost, es);
    g[to].emplace_back(to, from, cost, es++);
  }

  /// @brief エッジを読み込む関数 @param M エッジの数 @param padding インデックスのオフセット @param weighted 重み付きかどうか @param directed 有向かどうか
  void read(int M, int padding = -1, bool weighted = false,
            bool directed = false) {
    for (int i = 0; i < M; i++) {
      int a, b;
      std::cin >> a >> b;
      a += padding;
      b += padding;
      T c = T(1);
      if (weighted) std::cin >> c;
      if (directed)
        add_directed_edge(a, b, c);
      else
        add_edge(a, b, c);
    }
  }

  // 演算子オーバーロード：インデックスによるエッジのリストへのアクセス
  inline std::vector<Edge<T> > &operator[](const int &k) { return g[k]; }

  // 演算子オーバーロード（const版）：インデックスによるエッジのリストへのアクセス
  inline const std::vector<Edge<T> > &operator[](const int &k) const { return g[k]; }
};

/// @brief エッジのリスト @tparam T エッジの重みの型
template <typename T = int>
using Edges = std::vector<Edge<T> >;
