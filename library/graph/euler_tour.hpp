#pragma once

#include <algorithm>
#include <functional>
#include <unordered_map>
#include <utility>
#include <vector>

#include <atcoder/lazysegtree>
#include <atcoder/segtree>

// Authorship provenance: the user reports that @rrrrikiOW wrote this component
// with GitHub Copilot assistance. The legacy snippet names no concrete external
// source for this implementation. This record is not a warranty that every
// generated fragment is copyrightable or free of third-party claims.
// Copyright 2026 rrrriki. Copyrightable human-authored contributions are
// licensed under Apache-2.0. See LICENSE-SCOPE.md and THIRD_PARTY_NOTICES.md.
// AtCoder Library is an external CC0-1.0 dependency and is not vendored here.

template <typename T>
struct Graph;

template <typename E_T, typename V_T>
struct EulerTour {
 private:
  struct PE {
    E_T value;
    int rate;
    PE(E_T value, int rate) : value(value * rate), rate(rate) {}
    PE() : value(0), rate(0) {}
    static PE from_sum(E_T sum, int rate) {
      PE result;
      result.value = sum;
      result.rate = rate;
      return result;
    }
    E_T val() const { return value; }
  };
  struct PV {
    V_T value;
    int rate;
    PV(V_T value, int rate) : value(value * rate), rate(rate) {}
    PV() : value(0), rate(0) {}
    static PV from_sum(V_T sum, int rate) {
      PV result;
      result.value = sum;
      result.rate = rate;
      return result;
    }
    V_T val() const { return value; }
  };

  bool build_lca_flag = false;
  bool build_seg_edge_flag[2] = {false, false};
  bool build_seg_vert_flag[2] = {false, false};
  std::vector<V_T> vert_weight;
  std::unordered_map<long long, std::pair<int, int>> edge_to_index;
  std::vector<PE> edge_weight_tour, edge_weight_tour_minus;
  std::vector<PV> vert_weight_tour, vert_weight_tour_minus;
  std::vector<std::pair<int, int>> depth;
  using S = std::pair<int, int>;
  static S op_lca(S a, S b) { return std::min(a, b); }
  static S e_lca() { return {1e9, 1e9}; }
  atcoder::segtree<S, &EulerTour::op_lca, &EulerTour::e_lca> seg_lca;

  static PE op_edge(PE a, PE b) {
    return PE::from_sum(a.value + b.value, a.rate + b.rate);
  }
  static PE e_edge() { return PE(0, 0); }
  static PE mapping_edge(E_T f, PE x) {
    return PE::from_sum(x.value + f * x.rate, x.rate);
  }
  static E_T composition_edge(E_T f, E_T g) { return f + g; }
  static E_T id_edge() { return 0; }
  atcoder::lazy_segtree<PE, &EulerTour::op_edge, &EulerTour::e_edge, E_T,
                        &EulerTour::mapping_edge,
                        &EulerTour::composition_edge, &EulerTour::id_edge>
      seg_edge0, seg_edge1;

  static PV op_vert(PV a, PV b) {
    return PV::from_sum(a.value + b.value, a.rate + b.rate);
  }
  static PV e_vert() { return PV(0, 0); }
  static PV mapping_vert(V_T f, PV x) {
    return PV::from_sum(x.value + f * x.rate, x.rate);
  }
  static V_T composition_vert(V_T f, V_T g) { return f + g; }
  static V_T id_vert() { return 0; }
  atcoder::lazy_segtree<PV, &EulerTour::op_vert, &EulerTour::e_vert, V_T,
                        &EulerTour::mapping_vert,
                        &EulerTour::composition_vert, &EulerTour::id_vert>
      seg_vert0, seg_vert1;

  void build_lca() {
    seg_lca = atcoder::segtree<S, op_lca, e_lca>(depth);
    build_lca_flag = true;
  }
  void build_seg_edge(int ind) {
    build_seg_edge_flag[ind] = true;
    if (ind == 0)
      seg_edge0 = atcoder::lazy_segtree<
          PE, &EulerTour::op_edge, &EulerTour::e_edge, E_T,
          &EulerTour::mapping_edge, &EulerTour::composition_edge,
          &EulerTour::id_edge>(edge_weight_tour);
    else
      seg_edge1 = atcoder::lazy_segtree<
          PE, &EulerTour::op_edge, &EulerTour::e_edge, E_T,
          &EulerTour::mapping_edge, &EulerTour::composition_edge,
          &EulerTour::id_edge>(edge_weight_tour_minus);
  }
  void build_seg_vert(int ind) {
    build_seg_vert_flag[ind] = true;
    if (ind == 0)
      seg_vert0 = atcoder::lazy_segtree<
          PV, &EulerTour::op_vert, &EulerTour::e_vert, V_T,
          &EulerTour::mapping_vert, &EulerTour::composition_vert,
          &EulerTour::id_vert>(vert_weight_tour);
    else
      seg_vert1 = atcoder::lazy_segtree<
          PV, &EulerTour::op_vert, &EulerTour::e_vert, V_T,
          &EulerTour::mapping_vert, &EulerTour::composition_vert,
          &EulerTour::id_vert>(vert_weight_tour_minus);
  }

 public:
  std::vector<int> in, out;

  EulerTour(Graph<E_T>& g, int root = 0,
            std::vector<V_T> vert_w = std::vector<V_T>())
      : in(g.size()), out(g.size()) {
    if (vert_w.size() == 0)
      vert_weight = std::vector<V_T>(g.size(), 1);
    else
      vert_weight = vert_w;
    int idx = 0;
    edge_weight_tour.reserve(2 * g.size());
    edge_weight_tour_minus.reserve(2 * g.size());
    vert_weight_tour.reserve(2 * g.size());
    vert_weight_tour_minus.reserve(2 * g.size());
    edge_weight_tour.emplace_back(PE(0, 0));
    edge_weight_tour_minus.emplace_back(PE(0, 0));
    vert_weight_tour.emplace_back(PV(vert_weight[root], 1));
    vert_weight_tour_minus.emplace_back(PV(vert_weight[root], 1));
    depth.emplace_back(0, root);
    std::function<void(int, int, int)> dfs = [&](int v, int p, int d) {
      in[v] = idx++;
      for (auto e : g[v]) {
        if (e.to == p) continue;
        depth.emplace_back(d + 1, e.to);
        edge_weight_tour.emplace_back(PE(e.cost, 1));
        edge_weight_tour_minus.emplace_back(PE(e.cost, 1));
        vert_weight_tour.emplace_back(PV(vert_weight[e.to], 1));
        vert_weight_tour_minus.emplace_back(PV(vert_weight[e.to], 1));
        edge_to_index[std::min(v, static_cast<int>(e.to)) * 10000000000LL +
                      std::max(v, static_cast<int>(e.to))] = {idx, -1};
        dfs(e.to, v, d + 1);
        edge_to_index[std::min(v, static_cast<int>(e.to)) * 10000000000LL +
                      std::max(v, static_cast<int>(e.to))]
            .second = idx++;
        depth.emplace_back(d, v);
        edge_weight_tour.emplace_back(PE(0, 0));
        edge_weight_tour_minus.emplace_back(PE(e.cost, -1));
        vert_weight_tour.emplace_back(PV(0, 0));
        vert_weight_tour_minus.emplace_back(PV(vert_weight[e.to], -1));
      }
      out[v] = idx;
    };
    dfs(root, -1, 0);
  }

  int lca(int u, int v) {
    if (!build_lca_flag) build_lca();
    return seg_lca.prod(std::min(in[u], in[v]),
                        std::max(out[u], out[v]))
        .second;
  }

  E_T query_edge(int u, int v) {
    if (!build_seg_edge_flag[1]) build_seg_edge(1);
    if (!build_lca_flag) build_lca();
    return (seg_edge1.prod(0, in[u] + 1)).val() +
           (seg_edge1.prod(0, in[v] + 1)).val() -
           2 * (seg_edge1.prod(0, in[lca(u, v)] + 1)).val();
  }
  V_T query_vert(int u, int v) {
    if (!build_seg_vert_flag[1]) build_seg_vert(1);
    if (!build_lca_flag) build_lca();
    return (seg_vert1.prod(0, in[u] + 1)).val() +
           (seg_vert1.prod(0, in[v] + 1)).val() -
           2 * (seg_vert1.prod(0, in[lca(u, v)] + 1)).val();
  }
  E_T query_subtree_edge(int u) {
    if (!build_seg_edge_flag[0]) build_seg_edge(0);
    return (seg_edge0.prod(in[u] + 1, out[u])).val();
  }
  V_T query_subtree_vert(int u) {
    if (!build_seg_vert_flag[0]) build_seg_vert(0);
    return (seg_vert0.prod(in[u], out[u])).val();
  }

  void update_edge(int u, int v, E_T x, int mode = 1) {
    if (u > v) std::swap(u, v);
    if (mode != 1) {
      if (!build_seg_edge_flag[0]) build_seg_edge(0);
      seg_edge0.set(edge_to_index[u * 10000000000LL + v].first, PE(x, 1));
    }
    if (mode != 0) {
      if (!build_seg_edge_flag[1]) build_seg_edge(1);
      seg_edge1.set(edge_to_index[u * 10000000000LL + v].first, PE(x, 1));
      seg_edge1.set(edge_to_index[u * 10000000000LL + v].second, PE(x, -1));
    }
  }
  void add_cost_edge(int u, int v, E_T x, int mode = 1) {
    if (u > v) std::swap(u, v);
    if (mode != 1) {
      if (!build_seg_edge_flag[0]) build_seg_edge(0);
      seg_edge0.apply(edge_to_index[u * 10000000000LL + v].first, x);
    }
    if (mode != 0) {
      if (!build_seg_edge_flag[1]) build_seg_edge(1);
      seg_edge1.apply(edge_to_index[u * 10000000000LL + v].first, x);
      seg_edge1.apply(edge_to_index[u * 10000000000LL + v].second, x);
    }
  }
  void update_vert(int u, V_T x, int mode = 1) {
    if (mode != 1) {
      if (!build_seg_vert_flag[0]) build_seg_vert(0);
      seg_vert0.set(in[u], PV(x, 1));
    }
    if (mode != 0) {
      if (!build_seg_vert_flag[1]) build_seg_vert(1);
      seg_vert1.set(in[u], PV(x, 1));
      if (out[u] < static_cast<int>(vert_weight_tour_minus.size()))
        seg_vert1.set(out[u], PV(x, -1));
    }
  }
  void add_cost_vert(int u, V_T x, int mode = 1) {
    if (mode != 1) {
      if (!build_seg_vert_flag[0]) build_seg_vert(0);
      seg_vert0.apply(in[u], x);
    }
    if (mode != 0) {
      if (!build_seg_vert_flag[1]) build_seg_vert(1);
      seg_vert1.apply(in[u], x);
      if (out[u] < static_cast<int>(vert_weight_tour_minus.size()))
        seg_vert1.apply(out[u], x);
    }
  }
  void add_cost_subtree_vert(int u, V_T x, int mode = 1) {
    if (mode != 1) {
      if (!build_seg_vert_flag[0]) build_seg_vert(0);
      seg_vert0.apply(in[u], out[u], x);
    }
    if (mode != 0) {
      if (!build_seg_vert_flag[1]) build_seg_vert(1);
      const int right = std::min(out[u] + 1,
                                 static_cast<int>(vert_weight_tour_minus.size()));
      seg_vert1.apply(in[u], right, x);
    }
  }
  void add_cost_subtree_edge(int u, E_T x, int mode = 1) {
    if (mode != 1) {
      if (!build_seg_edge_flag[0]) build_seg_edge(0);
      seg_edge0.apply(in[u] + 1, out[u], x);
    }
    if (mode != 0) {
      if (!build_seg_edge_flag[1]) build_seg_edge(1);
      seg_edge1.apply(in[u] + 1, out[u], x);
    }
  }

  int get_depth(int u) { return depth[in[u]].first; }
  V_T get_vert(int u, int mode = 1) {
    if (mode == 0) {
      if (!build_seg_vert_flag[0]) build_seg_vert(0);
      return seg_vert0.get(in[u]).val();
    } else {
      if (!build_seg_vert_flag[1]) build_seg_vert(1);
      return seg_vert1.get(in[u]).val();
    }
  }
  E_T get_edge(int u, int v, int mode = 1) {
    if (u > v) std::swap(u, v);
    if (mode == 0) {
      if (!build_seg_edge_flag[0]) build_seg_edge(0);
      return seg_edge0.get(edge_to_index[u * 10000000000LL + v].first).val();
    } else {
      if (!build_seg_edge_flag[1]) build_seg_edge(1);
      return seg_edge1.get(edge_to_index[u * 10000000000LL + v].first).val();
    }
  }
};
