#pragma once
// The user reports creating this legacy @NQUEEN snippet with AI assistance.
// Copyright 2026 rrrriki. Copyrightable human-authored contributions are
// licensed under Apache-2.0; this provenance report does not guarantee rights
// in every generated fragment. See LICENSE-SCOPE.md and THIRD_PARTY_NOTICES.md.
#include <cassert>
#include <iostream>
#include <set>
#include <unordered_set>
#include <utility>
#include <vector>
using std::vector; using std::pair; using std::set; using std::unordered_set; using std::cout;

/**
 * @brief N-Queen問題を解くための構造体
 *
 * 使い方
 *
 * - NQueen nqueen(n):= n x n のチェス盤を宣言
 *
 * - add_queen(x, y):= (x, y) にクイーンを置く
 *
 * - remove_queen(x, y):= (x, y) のクイーンを取り除く
 *
 * - is_valid(x, y):= (x, y) にクイーンを置けるかどうか
 *
 * - is_valid():= クイーンが全てのマスを利き筋に置いているかどうか
 *
 * - solve(x):= x行目以降のクイーンを置く
 *
 * - size():= チェス盤のサイズを返す
 *
 * - get_queens():= クイーンの位置をvector<pair<int,int>>に出力する
 *
 * - get_added_queens():= 追加したクイーンの位置をvector<pair<int,int>>に出力する
 *
 * - print(c, d):= 盤面を出力する c: クイーンの文字 d: 空白の文字
 *
 */
struct NQueen {
 public:
  explicit NQueen(int n) : N(n) { assert(n > 0); }

  /// @brief クイーンを置く @param x x座標 @param y y座標 @return クイーンを置けたかどうか
  bool add_queen(int x, int y) {
    if (row.count(x) || col.count(y) || diag1.count(x + y) || diag2.count(x - y)) return false;
    assert(x < N && y < N);
    assert(x >= 0 && y >= 0);
    queens.emplace(x, y);
    row.insert(x);        // x が同じ行にある
    col.insert(y);        // y が同じ列にある
    diag1.insert(x + y);  // x + y が同じ斜めの利き筋にある
    diag2.insert(x - y);  // x - y が同じ斜めの利き筋にある
    return true;
  }

  /// @brief クイーンを取り除く @param x x座標 @param y y座標 @return クイーンを取り除けたかどうか
  bool remove_queen(int x, int y) {
    if (!queens.count({x, y})) return false;
    assert(x < N && y < N);
    assert(x >= 0 && y >= 0);
    queens.erase({x, y});
    if (added_queens.count({x, y})) added_queens.erase({x, y});
    row.erase(x);
    col.erase(y);
    diag1.erase(x + y);
    diag2.erase(x - y);
    return true;
  }

  /// @brief x, yにクイーンを置けるかどうか @param x x座標 @param y y座標 @return クイーンを置けるかどうか
  bool is_valid(int x, int y) { return !row.count(x) && !col.count(y) && !diag1.count(x + y) && !diag2.count(x - y); }

  /// @brief クイーンが全てのマスを利き筋に置いているかどうか
  inline bool is_valid() { return (int)row.size() == N; }

  /// @brief x行目以降のクイーンを置く @return クイーンを置けたかどうか
  bool solve(int x = 0) {
    if (x == N) return true;
    if (is_valid()) return true;
    if (row.count(x)) return solve(x + 1);
    for (int y = 0; y < N; y++) {
      if (is_valid(x, y)) {
        add_queen(x, y);
        added_queens.emplace(x, y);
        if (solve(x + 1)) return true;
        remove_queen(x, y);
      }
    }
    return false;
  }

  /// @brief チェス盤のサイズを返す
  inline int size() { return N; }

  /// @brief クイーンの位置を出力する
  friend std::ostream &operator<<(std::ostream &os, const NQueen &nqueen) {
    for (auto [x, y] : nqueen.queens) os << x << " " << y << "\n";
    return os;
  }

  /// @brief クイーンの位置をvector<pair<int,int>>に出力する
  inline vector<pair<int, int> > get_queens() { return vector<pair<int, int> >(queens.begin(), queens.end()); }

  /// @brief 追加したクイーンの位置をvector<pair<int,int>>に出力する
  inline vector<pair<int, int> > get_added_queens() { return vector<pair<int, int> >(added_queens.begin(), added_queens.end()); }

  /// @brief 盤面を出力する @param c クイーンの文字 @param d 空白の文字 @note デフォルトは 'Q' と '.'
  void print(char c = 'Q', char d = '.') {
    vector<vector<char> > board(N, vector<char>(N, d));
    for (auto [x, y] : queens) {
      board[x][y] = c;
    }
    for (auto &row : board) {
      for (auto &c : row) {
        cout << c;
      }
      cout << "\n";
    }
  }

 private:
  int N;                                      // チェス盤のサイズ
  unordered_set<int> row, col, diag1, diag2;  // それぞれの行、列、斜めの利き筋にクイーンがあるかどうか
  set<pair<int, int> > queens;                // クイーンの位置
  set<pair<int, int> > added_queens;          // 追加したクイーンの位置
};
