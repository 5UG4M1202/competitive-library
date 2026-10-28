#pragma once

#include <algorithm>
#include <cstdint>
#include <utility>
#include <vector>

// Adapted from NyaanNyaan/library's rbst-base.hpp and
// lazy-reversible-rbst.hpp at commit b3981adc80a800b2584980b01821324ea6c77183
// (CC0-1.0):
// https://github.com/NyaanNyaan/library/tree/b3981adc80a800b2584980b01821324ea6c77183/rbst
// Copyright 2026 rrrriki for copyrightable local additions and modifications,
// licensed under Apache-2.0. Upstream CC0 rights remain. See
// THIRD_PARTY_NOTICES.md and LICENSE-SCOPE.md.

template <typename Node>
struct RBSTBase {
  using Ptr = Node*;

  template <typename... Args>
  inline Ptr my_new(Args... args) {
    return new Node(args...);
  }

  inline void my_del(Ptr t) { delete t; }
  inline Ptr make_tree() const { return nullptr; }

  // For avoiding memory leaks, the original snippet proposed replacing raw
  // pointers with shared_ptr. The public API retains raw pointers for
  // compatibility; callers own the remaining nodes.

  int size(Ptr t) const { return count(t); }

  Ptr merge(Ptr l, Ptr r) {
    if (!l || !r) return l ? l : r;
    if (int((rng() * (l->cnt + r->cnt)) >> 32) < l->cnt) {
      push(l);
      l->r = merge(l->r, r);
      return update(l);
    } else {
      push(r);
      r->l = merge(l, r->l);
      return update(r);
    }
  }

  std::pair<Ptr, Ptr> split(Ptr t, int k) {
    if (!t) return {nullptr, nullptr};
    push(t);
    if (k <= count(t->l)) {
      auto s = split(t->l, k);
      t->l = s.second;
      return {s.first, update(t)};
    } else {
      auto s = split(t->r, k - count(t->l) - 1);
      t->r = s.first;
      return {update(t), s.second};
    }
  }

  Ptr build(int l, int r, const std::vector<decltype(Node::key)>& v) {
    if (l + 1 == r) return my_new(v[l]);
    int m = (l + r) >> 1;
    Ptr pm = my_new(v[m]);
    if (l < m) pm->l = build(l, m, v);
    if (m + 1 < r) pm->r = build(m + 1, r, v);
    return update(pm);
  }

  Ptr build(const std::vector<decltype(Node::key)>& v) {
    return build(0, static_cast<int>(v.size()), v);
  }

  template <typename... Args>
  void insert(Ptr& t, int k, const Args&... args) {
    auto x = split(t, k);
    t = merge(merge(x.first, my_new(args...)), x.second);
  }

  void erase(Ptr& t, int k) {
    auto x = split(t, k);
    auto y = split(x.second, 1);
    my_del(y.first);
    t = merge(x.first, y.second);
  }

 protected:
  static std::uint64_t rng() {
    static std::uint64_t x_ = 88172645463325252ULL;
    return x_ ^= x_ << 7, x_ ^= x_ >> 9, x_ & 0xFFFFFFFFull;
  }

  inline int count(const Ptr t) const { return t ? t->cnt : 0; }

  virtual void push(Ptr) = 0;
  virtual Ptr update(Ptr) = 0;
};

template <typename T, typename E>
struct LazyReversibleRBSTNode {
  typename RBSTBase<LazyReversibleRBSTNode>::Ptr l, r;
  T key, sum;
  E lazy;
  int cnt;
  bool rev;

  LazyReversibleRBSTNode(const T& t = T(), const E& e = E())
      : l(), r(), key(t), sum(t), lazy(e), cnt(1), rev(false) {}
};

template <typename T, typename E, T (*f)(T, T), T (*g)(T, E), E (*h)(E, E),
          T (*ts)(T)>
struct LazyReversibleRBST : RBSTBase<LazyReversibleRBSTNode<T, E>> {
  using Node = LazyReversibleRBSTNode<T, E>;
  using base = RBSTBase<LazyReversibleRBSTNode<T, E>>;
  using base::merge;
  using base::split;
  using typename base::Ptr;

  LazyReversibleRBST() = default;

  void toggle(Ptr t) {
    if (!t) return;
    std::swap(t->l, t->r);
    t->sum = ts(t->sum);
    t->rev ^= true;
  }

  T fold(Ptr& t, int a, int b) {
    auto x = split(t, a);
    auto y = split(x.second, b - a);
    auto ret = sum(y.first);
    t = merge(x.first, merge(y.first, y.second));
    return ret;
  }

  void reverse(Ptr& t, int a, int b) {
    auto x = split(t, a);
    auto y = split(x.second, b - a);
    toggle(y.first);
    t = merge(x.first, merge(y.first, y.second));
  }

  void apply(Ptr& t, int a, int b, const E& e) {
    auto x = split(t, a);
    auto y = split(x.second, b - a);
    propagate(y.first, e);
    t = merge(x.first, merge(y.first, y.second));
  }

 protected:
  inline T sum(const Ptr t) const { return t ? t->sum : T(); }

  Ptr update(Ptr t) override {
    push(t);
    t->cnt = 1;
    t->sum = t->key;
    if (t->l) t->cnt += t->l->cnt, t->sum = f(t->l->sum, t->sum);
    if (t->r) t->cnt += t->r->cnt, t->sum = f(t->sum, t->r->sum);
    return t;
  }

  void push(Ptr t) override {
    if (t->rev) {
      if (t->l) toggle(t->l);
      if (t->r) toggle(t->r);
      t->rev = false;
    }
    if (t->lazy != E()) {
      if (t->l) propagate(t->l, t->lazy);
      if (t->r) propagate(t->r, t->lazy);
      t->lazy = E();
    }
  }

  void propagate(Ptr t, const E& x) {
    t->lazy = h(t->lazy, x);
    t->key = g(t->key, x);
    t->sum = g(t->sum, x);
  }
};
