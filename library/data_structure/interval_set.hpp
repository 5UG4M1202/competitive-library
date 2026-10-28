#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <limits>
#include <ostream>
#include <set>
#include <vector>

// Portions adapted from drken1215/algorithm's IntervalSet (CC0-1.0):
// https://github.com/drken1215/algorithm/blob/79b5a6a826690c4d8e0bd64e812e85b5f425ea6b/DataStructureBasic/intervals_set.cpp
// Copyright 2026 rrrriki for local additions and modifications, licensed under
// Apache-2.0. Upstream CC0 rights remain. See THIRD_PARTY_NOTICES.md and
// LICENSE-SCOPE.md. This is a legacy-compatible extraction from cpp.json's
// @interval_set snippet. Managed intervals are half-open: [l, r).
template <class T, class VAL = long long>
struct IntervalSet {
  struct Node {
    T l, r;
    VAL val;

    Node(const T& l_, const T& r_, const VAL& val_)
        : l(l_), r(r_), val(val_) {}

    bool operator<(const Node& rhs) const {
      if (l != rhs.l) return l < rhs.l;
      return r < rhs.r;
    }

    friend std::ostream& operator<<(std::ostream& s, const Node& e) {
      return s << "([" << e.l << ", " << e.r << "): " << e.val << ")";
    }
  };

  const VAL identity;
  std::set<Node> S;
  T total_cover = 0;

  IntervalSet(const VAL& identity_ = VAL()) : identity(identity_) {}

  IntervalSet(const std::vector<VAL>& v, const VAL& identity_ = VAL())
      : identity(identity_) {
    std::vector<Node> vec;
    int n = static_cast<int>(v.size());
    for (int l = 0; l < n;) {
      int r = l;
      while (r < n && v[r] == v[l]) ++r;
      vec.emplace_back(static_cast<T>(l), static_cast<T>(r), v[l]);
      total_cover += static_cast<T>(r - l);
      l = r;
    }
    S = std::set<Node>(vec.begin(), vec.end());
  }

  auto begin() { return S.begin(); }
  auto end() { return S.end(); }
  auto begin() const { return S.begin(); }
  auto end() const { return S.end(); }
  std::size_t size() const { return S.size(); }

  auto get(const T& p) {
    auto it =
        S.upper_bound(Node(p, std::numeric_limits<T>::max(), identity));
    if (it == S.begin()) return S.end();
    --it;
    if (it->l <= p && p < it->r) return it;
    return S.end();
  }

  auto get(const T& p) const {
    auto it =
        S.upper_bound(Node(p, std::numeric_limits<T>::max(), identity));
    if (it == S.begin()) return S.end();
    --it;
    if (it->l <= p && p < it->r) return it;
    return S.end();
  }

  auto lower_bound(const T& p) {
    auto it = get(p);
    if (it != S.end()) return it;
    return S.upper_bound(Node(p, std::numeric_limits<T>::max(), identity));
  }

  auto lower_bound(const T& p) const {
    auto it = get(p);
    if (it != S.end()) return it;
    return S.upper_bound(Node(p, std::numeric_limits<T>::max(), identity));
  }

  bool covered(const T& p) { return get(p) != S.end(); }

  bool covered(const T& l, const T& r) {
    assert(l <= r);
    if (l == r) return true;
    auto it = get(l);
    return it != S.end() && r <= it->r;
  }

  bool same(const T& p, const T& q) {
    auto itp = get(p);
    if (itp == S.end()) return false;
    auto itq = get(q);
    if (itq == S.end()) return false;
    return itp == itq;
  }

  VAL get_val(const T& p) const {
    auto it = get(p);
    if (it != S.end()) return it->val;
    return identity;
  }

  VAL operator[](const T& p) const { return get_val(p); }

  T get_mex(const T& p = 0) const {
    auto it =
        S.upper_bound(Node(p, std::numeric_limits<T>::max(), identity));
    if (it == S.begin()) return p;
    --it;
    if (it->l <= p && p < it->r) return it->r;
    return p;
  }

  T covered_all() const { return total_cover; }

  T covered_length_in(const T& L, const T& R) const {
    if (L >= R) return 0;
    T res = 0;
    auto it =
        S.lower_bound(Node(L, std::numeric_limits<T>::max(), identity));
    if (it != S.begin()) {
      auto it2 = std::prev(it);
      if (it2->r > L) {
        T a = std::max(L, it2->l);
        T b = std::min(R, it2->r);
        if (a < b) res += b - a;
      }
    }
    for (; it != S.end() && it->l < R; ++it) {
      T a = std::max(L, it->l);
      T b = std::min(R, it->r);
      if (a < b) res += b - a;
    }
    return res;
  }

  long long contained_intervals_in(const T& L, const T& R) const {
    if (L >= R) return 0;
    long long cnt = 0;
    auto it = S.lower_bound(Node(L, L, identity));
    for (; it != S.end() && it->l < R; ++it) {
      if (L <= it->l && it->r <= R) ++cnt;
    }
    return cnt;
  }

  template <class ADDFUNC, class DELFUNC>
  void update(T l, T r, const VAL& val, const ADDFUNC& user_add,
              const DELFUNC& user_del) {
    if (l == r) return;

    auto add = [&](T L, T R, VAL V) {
      total_cover += R - L;
      user_add(L, R, V);
    };
    auto del = [&](T L, T R, VAL V) {
      total_cover -= R - L;
      user_del(L, R, V);
    };

    auto it = S.lower_bound(Node(l, T{}, val));
    while (it != S.end() && it->l <= r) {
      if (it->l == r) {
        if (it->val == val) {
          r = it->r;
          del(it->l, it->r, it->val);
          it = S.erase(it);
        }
        break;
      }
      if (it->r <= r) {
        del(it->l, it->r, it->val);
        it = S.erase(it);
      } else {
        if (it->val == val) {
          r = it->r;
          del(it->l, it->r, it->val);
          it = S.erase(it);
        } else {
          Node node = *it;
          del(it->l, it->r, it->val);
          it = S.erase(it);
          it = S.emplace_hint(it, r, node.r, node.val);
          add(it->l, it->r, it->val);
        }
      }
    }
    if (it != S.begin()) {
      it = std::prev(it);
      if (it->r == l) {
        if (it->val == val) {
          l = it->l;
          del(it->l, it->r, it->val);
          it = S.erase(it);
        }
      } else if (l < it->r) {
        if (it->val == val) {
          l = std::min(l, it->l);
          r = std::max(r, it->r);
          del(it->l, it->r, it->val);
          it = S.erase(it);
        } else {
          if (r < it->r) {
            it = S.emplace_hint(std::next(it), r, it->r, it->val);
            add(it->l, it->r, it->val);
            it = std::prev(it);
          }
          Node node = *it;
          del(it->l, it->r, it->val);
          it = S.erase(it);
          it = S.emplace_hint(it, node.l, l, node.val);
          add(it->l, it->r, it->val);
        }
      }
    }
    if (it != S.end()) ++it;
    it = S.emplace_hint(it, l, r, val);
    add(it->l, it->r, it->val);
  }

  void update(const T& l, const T& r, const VAL& val) {
    update(l, r, val, [](T, T, VAL) {}, [](T, T, VAL) {});
  }

  template <class ADDFUNC, class DELFUNC>
  void insert(T l, T r, const ADDFUNC& add, const DELFUNC& del) {
    update(l, r, VAL(), add, del);
  }

  void insert(const T& l, const T& r) {
    update(l, r, VAL(), [](T, T, VAL) {}, [](T, T, VAL) {});
  }

  template <class ADDFUNC, class DELFUNC>
  void erase(T l, T r, const ADDFUNC& user_add,
             const DELFUNC& user_del) {
    if (l == r) return;

    auto add = [&](T L, T R, VAL V) {
      total_cover += R - L;
      user_add(L, R, V);
    };
    auto del = [&](T L, T R, VAL V) {
      total_cover -= R - L;
      user_del(L, R, V);
    };

    auto it = S.lower_bound(Node(l, T{}, VAL()));
    while (it != S.end() && it->l <= r) {
      if (it->l == r) break;
      if (it->r <= r) {
        del(it->l, it->r, it->val);
        it = S.erase(it);
      } else {
        Node node = *it;
        del(it->l, it->r, it->val);
        it = S.erase(it);
        it = S.emplace_hint(it, r, node.r, node.val);
        add(it->l, it->r, it->val);
      }
    }
    if (it != S.begin()) {
      it = std::prev(it);
      if (l < it->r) {
        if (r < it->r) {
          it = S.emplace_hint(std::next(it), r, it->r, it->val);
          add(it->l, it->r, it->val);
          it = std::prev(it);
        }
        Node node = *it;
        del(it->l, it->r, it->val);
        it = S.erase(it);
        it = S.emplace_hint(it, node.l, l, node.val);
        add(it->l, it->r, it->val);
      }
    }
  }

  void erase(const T& l, const T& r) {
    erase(l, r, [](T, T, VAL) {}, [](T, T, VAL) {});
  }

  friend std::ostream& operator<<(std::ostream& s, const IntervalSet& ins) {
    for (const auto& e : ins.S) {
      s << "([" << e.l << ", " << e.r << "): " << e.val << ") ";
    }
    return s;
  }
};
