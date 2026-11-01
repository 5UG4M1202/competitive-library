#pragma once

#include <algorithm>
#include <cassert>
#include <functional>
#include <memory>
#include <utility>
#include <vector>

// Extracted from cpp.json cpptemplate; user recalls likely AI generation.
// User-reported reference: https://atcoder.jp/contests/abc435/editorial/14743
// Apache-2.0 applies to rrrriki's copyrightable human-authored contributions.
// AI generation and third-party rights are not guaranteed by this report.
// See LICENSE-SCOPE.md and THIRD_PARTY_NOTICES.md.
template <class S,
          S (*op)(S, S),
          S (*e)(),
          class F,
          S (*mapping)(F, S),
          F (*composition)(F, F),
          F (*id)()>
struct dynamic_lazy_segtree {
  struct Node {
    S val;   // この節点区間のモノイド値
    F lazy;  // 遅延作用素
    Node* ch[2];
    Node(const S& v, const F& lz) : val(v), lazy(lz), ch{nullptr, nullptr} {}
  };

  using T = long long;  // 座標の型

  Node* root;
  T L_all, R_all;  // 管理範囲 [L_all, R_all)

  // 区間 [L, R) の初期値を返す関数
  std::function<S(T, T)> init;

  dynamic_lazy_segtree(T L, T R,
                       std::function<S(T, T)> init_ = nullptr)
      : root(nullptr), L_all(L), R_all(R) {
    assert(L_all < R_all);
    if (init_) {
      init = std::move(init_);
    } else {
      // デフォルトは全部 e()
      init = [](T, T) { return e(); };
    }
  }

  // Copies previously aliased raw nodes; unique ownership prevents aliasing.
  dynamic_lazy_segtree(const dynamic_lazy_segtree&) = delete;
  dynamic_lazy_segtree& operator=(const dynamic_lazy_segtree&) = delete;

  dynamic_lazy_segtree(dynamic_lazy_segtree&& other) noexcept
      : root(std::exchange(other.root, nullptr)), L_all(other.L_all),
        R_all(other.R_all), init(std::move(other.init)), nodes(std::move(other.nodes)) {}

  dynamic_lazy_segtree& operator=(dynamic_lazy_segtree&& other) noexcept {
    if (this != &other) {
      nodes = std::move(other.nodes);
      root = std::exchange(other.root, nullptr);
      L_all = other.L_all;
      R_all = other.R_all;
      init = std::move(other.init);
    }
    return *this;
  }

  /// @brief [l, r) に f を作用させる
  void apply(T l, T r, F f) {
    assert(L_all <= l && l <= r && r <= R_all);
    if (l == r) return;
    root = apply_impl(root, L_all, R_all, l, r, f);
  }

  /// @brief [l, r) のモノイド値を取得する
  S prod(T l, T r) {
    assert(L_all <= l && l <= r && r <= R_all);
    if (l == r) return e();
    return prod_impl(root, L_all, R_all, l, r);
  }

  /// @brief 全体 [L_all, R_all) のモノイド値
  S all_prod() {
    if (!root) return init(L_all, R_all);
    return root->val;
  }
  /// @brief 区間 [l, r) を左から伸ばしていき、条件 g が初めて破れる位置を返す
  /// 返り値 x は l <= x <= R_all で、次を満たす:
  ///   - g(prod(l, x)) = true
  ///   - x = R_all もしくは g(prod(l, x + 1)) = false
  template <bool (*g)(S)>
  T max_right(T l) {
    return max_right(l, [](S x) { return g(x); });
  }

  template <class G>
  T max_right(T l, G g) {
    assert(L_all <= l && l <= R_all);
    assert(g(e()));
    if (l == R_all) return R_all;

    S sm = e();
    return max_right_impl(root, L_all, R_all, l, g, sm);
  }

  /// @brief 区間 [l, r) を右から伸ばしていき、条件 g が初めて破れる位置を返す
  /// 返り値 x は L_all <= x <= r で、次を満たす:
  ///   - g(prod(x, r)) = true
  ///   - x = L_all もしくは g(prod(x - 1, r)) = false
  template <bool (*g)(S)>
  T min_left(T r) {
    return min_left(r, [](S x) { return g(x); });
  }

  template <class G>
  T min_left(T r, G g) {
    assert(L_all <= r && r <= R_all);
    assert(g(e()));
    if (r == L_all) return L_all;

    S sm = e();
    return min_left_impl(root, L_all, R_all, r, g, sm);
  }

 private:
  std::vector<std::unique_ptr<Node>> nodes;

  Node* new_node(T L, T R) {
    nodes.push_back(std::make_unique<Node>(init(L, R), id()));
    return nodes.back().get();
  }

  // 節点に遅延作用 f を全部かける
  static void all_apply(Node* node, F f) {
    if (!node) return;
    node->val = mapping(f, node->val);
    node->lazy = composition(f, node->lazy);
  }

  // 子に lazy を押し出す
  void push(Node* node, T L, T R) {
    if (!node) return;
    if (node->lazy == id()) return;
    T M = L + ((R - L) >> 1);
    if (!node->ch[0]) node->ch[0] = new_node(L, M);
    if (!node->ch[1]) node->ch[1] = new_node(M, R);
    all_apply(node->ch[0], node->lazy);
    all_apply(node->ch[1], node->lazy);
    node->lazy = id();
  }

  // 子から val を再計算（子がないところは init で埋める）
  void pull(Node* node, T L, T R) {
    if (!node) return;
    T M = L + ((R - L) >> 1);
    S lv = node->ch[0] ? node->ch[0]->val : init(L, M);
    S rv = node->ch[1] ? node->ch[1]->val : init(M, R);
    node->val = op(lv, rv);
  }

  Node* apply_impl(Node* node, T L, T R, T ql, T qr, F f) {
    if (qr <= L || R <= ql) return node;  // 完全に外
    if (!node) node = new_node(L, R);
    if (ql <= L && R <= qr) {  // 完全に内
      all_apply(node, f);
      return node;
    }
    push(node, L, R);
    T M = L + ((R - L) >> 1);
    node->ch[0] = apply_impl(node->ch[0], L, M, ql, qr, f);
    node->ch[1] = apply_impl(node->ch[1], M, R, ql, qr, f);
    pull(node, L, R);
    return node;
  }

  S prod_impl(Node* node, T L, T R, T ql, T qr) {
    if (qr <= L || R <= ql) return e();
    if (!node) {
      T l = std::max(L, ql);
      T r = std::min(R, qr);
      if (l >= r) return e();
      return init(l, r);  // まだ一度も触っていない区間は初期値
    }
    if (ql <= L && R <= qr) return node->val;
    push(node, L, R);
    T M = L + ((R - L) >> 1);
    S lv = prod_impl(node->ch[0], L, M, ql, qr);
    S rv = prod_impl(node->ch[1], M, R, ql, qr);
    return op(lv, rv);
  }

  template <class G>
  T max_right_impl(Node*& node, T L, T R, T ql, G& g, S& sm) {
    if (R <= ql) return ql;
    if (!node) node = new_node(L, R);

    if (ql <= L) {
      S v = op(sm, node->val);
      if (g(v)) {
        sm = v;
        return R;
      }
    }

    if (R - L == 1) return L;

    push(node, L, R);
    T M = L + ((R - L) >> 1);
    T res = max_right_impl(node->ch[0], L, M, ql, g, sm);
    if (res < M) return res;
    return max_right_impl(node->ch[1], M, R, ql, g, sm);
  }

  template <class G>
  T min_left_impl(Node*& node, T L, T R, T qr, G& g, S& sm) {
    if (qr <= L) return qr;
    if (!node) node = new_node(L, R);

    if (R <= qr) {
      S v = op(node->val, sm);
      if (g(v)) {
        sm = v;
        return L;
      }
    }

    if (R - L == 1) return R;

    push(node, L, R);
    T M = L + ((R - L) >> 1);
    T res = min_left_impl(node->ch[1], M, R, qr, g, sm);
    if (res > M) return res;
    return min_left_impl(node->ch[0], L, M, qr, g, sm);
  }
};
