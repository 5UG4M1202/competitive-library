#pragma once
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

// Extracted from legacy cpptemplate; historical provenance not established.
// Apache-2.0 applies only to rrrriki's copyrightable human contributions.
template <typename T = char>
struct GRID {
  static constexpr long long INF_VALUE = 1000000000000000000LL;
  std::vector<std::vector<T> > field;   // グリッドデータ
  std::unordered_set<T> wall_set;  // 通れないマスの集合
  std::vector<std::vector<bool> > vis;  // 到達確認用
  const std::vector<std::pair<int, int> > directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
  int H = 0, W = 0;  // 高さと幅

  /// @brief デフォルトコンストラクタ
  GRID() = default;
  /// @brief コンストラクタ @param h 高さ @param w 幅
  GRID(int h, int w, T default_value = '.') : H(h), W(w) {
    field.assign(H, std::vector<T>(W, default_value));
    vis.assign(H, std::vector<bool>(W, false));
  }
  /// @brief 通れないマスを追加 @param wall_obj 通れないマス
  void wall(T wall_obj) {
    wall_set.emplace(wall_obj);
  }
  /// @brief グリッドの読み込み @param h 高さ @param w 幅 @param default_wall 通れないマスのデフォルト値
  void read(int h, int w, T default_wall = '#') {
    H = h;
    W = w;
    field.assign(H, std::vector<T>(W));
    vis.assign(H, std::vector<bool>(W, false));  // visを初期化
    wall_set.emplace(default_wall);
    for (int i = 0; i < H; i++) {
      for (int j = 0; j < W; j++) {
        std::cin >> field[i][j];
      }
    }
  }
  /// @brief (si, sj) から各マスへの最短距離を求める @param si 始点の行 @param sj 始点の列 @return (si, sj) から各マスへの最短距離
  std::vector<std::vector<long long> > bfs(int si, int sj) const {
    std::vector<std::vector<long long> > dist(H, std::vector<long long>(W, INF_VALUE));
    if (si < 0 || si >= H || sj < 0 || sj >= W || wall_set.count(field[si][sj])) return dist;
    std::queue<std::pair<int, int> > q;
    dist[si][sj] = 0;
    q.push({si, sj});
    // BFS処理
    while (!q.empty()) {
      auto [i, j] = q.front();
      q.pop();
      for (const auto &[di, dj] : directions) {
        int ni = i + di, nj = j + dj;
        if (ni < 0 || ni >= H || nj < 0 || nj >= W) continue;  // 範囲外
        if (wall_set.count(field[ni][nj])) continue;           // 通れないマス
        if (dist[ni][nj] != INF_VALUE) continue;                     // 既に訪問済み
        dist[ni][nj] = dist[i][j] + 1;
        q.push({ni, nj});
      }
    }
    return dist;
  }
  /// @brief BFSを1つの始点から開始し、到達可能領域を探索 @param si 始点の行 @param sj 始点の列
  void bfs_vis(int si, int sj) {
    if (si < 0 || si >= H || sj < 0 || sj >= W || wall_set.count(field[si][sj]) || vis[si][sj]) {
      return;  // 無効な始点なら探索しない
    }
    std::queue<std::pair<int, int> > q;
    vis[si][sj] = true;
    q.push({si, sj});
    // BFS処理
    while (!q.empty()) {
      auto [i, j] = q.front();
      q.pop();
      for (const auto &[di, dj] : directions) {
        int ni = i + di, nj = j + dj;
        if (ni < 0 || ni >= H || nj < 0 || nj >= W) continue;        // 範囲外
        if (wall_set.count(field[ni][nj]) || vis[ni][nj]) continue;  // 壁または訪問済み
        vis[ni][nj] = true;
        q.push({ni, nj});
      }
    }
  }
  /// @brief (i, j) の要素にアクセス @param i 行 @param j 列 @return (i, j) の要素
  std::vector<T> &operator[](int i) {
    return field[i];
  }
  const std::vector<T> &operator[](int i) const {
    return field[i];
  }
  /// @brief グリッドの出力 @param gap 区切り文字
  void print(std::string gap = "") {
    for (int i = 0; i < H; i++) {
      for (int j = 0; j < W; j++) {
        std::cout << field[i][j];
        if (j < W - 1) std::cout << gap;
      }
      std::cout << "\n";
    }
  }
  /// @brief 座標の取得 @param obj オブジェクト @return オブジェクトの座標
  std::vector<std::pair<int, int> > get_positions(T obj = '.') {
    std::vector<std::pair<int, int> > positions;
    for (int i = 0; i < H; i++) {
      for (int j = 0; j < W; j++) {
        if (field[i][j] == obj) {
          positions.emplace_back(i, j);
        }
      }
    }
    return positions;
  }
  /// @brief スタートとゴールの座標を取得 @param start スタートのオブジェクト @param goal ゴールのオブジェクト @return スタートとゴールの座標
  std::pair<std::pair<int, int>, std::pair<int, int> > get_start_goal(T start = 'S', T goal = 'G') {
    std::pair<int, int> s, g;
    for (int i = 0; i < H; i++) {
      for (int j = 0; j < W; j++) {
        if (field[i][j] == start) {
          s = {i, j};
        }
        if (field[i][j] == goal) {
          g = {i, j};
        }
      }
    }
    return {s, g};
  }
};
