#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <limits>
#include <utility>
#include <vector>

namespace cp {

// A segment is traversed in increasing index order unless reversed is true.
struct HldSegment {
  int l, r;
  bool reversed;
};

// A static, connected, simple undirected tree. Construction does not mutate g.
class HeavyLightDecomposition {
 public:
  template <class Graph>
  explicit HeavyLightDecomposition(const Graph& g, int root = 0)
      : n_(static_cast<int>(g.size())), root_(root), parent_(n_, -1), depth_(n_, 0),
        size_(n_, 1), heavy_(n_, -1), head_(n_), index_(n_), vertex_(n_) {
    assert(g.size() <= static_cast<std::size_t>(std::numeric_limits<int>::max()));
    assert(n_ > 0 && 0 <= root && root < n_);
    std::vector<int> order;
    order.reserve(n_);
    std::vector<int> stack{root};
    while (!stack.empty()) {
      int v = stack.back();
      stack.pop_back();
      order.push_back(v);
      for (const auto& edge : g[v]) {
        int to = static_cast<int>(edge);
        assert(0 <= to && to < n_);
        if (to == parent_[v]) continue;
        parent_[to] = v;
        depth_[to] = depth_[v] + 1;
        stack.push_back(to);
      }
    }
    assert(static_cast<int>(order.size()) == n_);
    for (auto it = order.rbegin(); it != order.rend(); ++it) {
      int v = *it;
      if (parent_[v] == -1) continue;
      int p = parent_[v];
      size_[p] += size_[v];
      if (heavy_[p] == -1 || size_[v] > size_[heavy_[p]]) heavy_[p] = v;
    }
    std::vector<std::pair<int, int>> pending{{root, root}};
    int next = 0;
    while (!pending.empty()) {
      auto [v, h] = pending.back();
      pending.pop_back();
      head_[v] = h;
      index_[v] = next;
      vertex_[next++] = v;
      for (const auto& edge : g[v]) {
        int to = static_cast<int>(edge);
        if (parent_[to] == v && to != heavy_[v]) pending.emplace_back(to, to);
      }
      if (heavy_[v] != -1) pending.emplace_back(heavy_[v], h);
    }
    assert(next == n_);
  }

  int size() const { return n_; }
  int root() const { return root_; }
  int parent(int v) const { check_vertex(v); return parent_[v]; }
  int depth(int v) const { check_vertex(v); return depth_[v]; }
  int index(int v) const { check_vertex(v); return index_[v]; }
  int vertex(int i) const { assert(0 <= i && i < n_); return vertex_[i]; }
  int subtree_size(int v) const { check_vertex(v); return size_[v]; }
  std::pair<int, int> subtree_vertices(int v) const {
    check_vertex(v);
    return {index_[v], index_[v] + size_[v]};
  }
  std::pair<int, int> subtree_edges(int v) const {
    auto [l, r] = subtree_vertices(v);
    return {l + 1, r};
  }
  int lca(int u, int v) const {
    check_vertex(u); check_vertex(v);
    while (head_[u] != head_[v]) {
      if (depth_[head_[u]] > depth_[head_[v]]) u = parent_[head_[u]];
      else v = parent_[head_[v]];
    }
    return depth_[u] < depth_[v] ? u : v;
  }
  std::vector<HldSegment> path_vertices(int u, int v) const { return path(u, v, false); }
  std::vector<HldSegment> path_edges(int u, int v) const { return path(u, v, true); }
  // Calls visit in u->v order without dynamically allocating path storage.
  template <class Visit>
  void for_each_path_vertices(int u, int v, Visit&& visit) const {
    for_each_path(u, v, false, visit);
  }
  template <class Visit>
  void for_each_path_edges(int u, int v, Visit&& visit) const {
    for_each_path(u, v, true, visit);
  }

 private:
  int n_, root_;
  std::vector<int> parent_, depth_, size_, heavy_, head_, index_, vertex_;
  void check_vertex(int v) const { assert(0 <= v && v < n_); }
  std::vector<HldSegment> path(int u, int v, bool edges) const {
    // Every light edge at least halves the remaining subtree. Both sides
    // therefore fit in a buffer bounded by the number of bits in int.
    std::array<HldSegment, 2 * std::numeric_limits<int>::digits + 1> segments;
    std::size_t count = 0;
    auto append = [&](HldSegment segment) {
      assert(count < segments.size());
      segments[count++] = segment;
    };
    for_each_path(u, v, edges, append);
    return std::vector<HldSegment>(segments.begin(), segments.begin() + count);
  }
  template <class Visit>
  void for_each_path(int u, int v, bool edges, Visit& visit) const {
    check_vertex(u); check_vertex(v);
    // At most floor(log2(n)) light edges on the v side; n fits in int.
    std::array<HldSegment, std::numeric_limits<int>::digits + 1> right;
    std::size_t right_size = 0;
    while (head_[u] != head_[v]) {
      if (depth_[head_[u]] > depth_[head_[v]]) {
        visit(HldSegment{index_[head_[u]], index_[u] + 1, true});
        u = parent_[head_[u]];
      } else {
        assert(right_size < right.size());
        right[right_size++] = {index_[head_[v]], index_[v] + 1, false};
        v = parent_[head_[v]];
      }
    }
    if (depth_[u] > depth_[v]) {
      int l = index_[v] + static_cast<int>(edges), r = index_[u] + 1;
      if (l < r) visit(HldSegment{l, r, true});
    } else {
      int l = index_[u] + static_cast<int>(edges), r = index_[v] + 1;
      if (l < r) visit(HldSegment{l, r, false});
    }
    while (right_size > 0) visit(right[--right_size]);
  }
};

// M::op(a,b) concatenates the visited sequence a then b.
template <class M>
struct BidirectionalMonoid {
  using S = typename M::value_type;
  struct value_type { S forward, backward; };
  static value_type op(value_type a, value_type b) {
    return {M::op(a.forward, b.forward), M::op(b.backward, a.backward)};
  }
  static value_type id() { return {M::id(), M::id()}; }
  static value_type vertex(const S& value) { return {value, value}; }
  static value_type edge(const S& down, const S& up) { return {down, up}; }
};

// A non-owning adapter; use separate segment trees for vertex and edge data.
template <class M, class SegmentTree>
class HldPathFold {
 public:
  using S = typename M::value_type;
  using B = BidirectionalMonoid<M>;
  HldPathFold(const HeavyLightDecomposition& hld, SegmentTree& seg) : hld_(hld), seg_(seg) {}
  HldPathFold(const HeavyLightDecomposition&&, SegmentTree&) = delete;
  void set_vertex(int v, const S& value) { seg_.set(hld_.index(v), B::vertex(value)); }
  // uv is u->v and vu is v->u; u and v must be adjacent.
  void set_edge(int u, int v, const S& uv, const S& vu) {
    if (hld_.parent(v) == u) seg_.set(hld_.index(v), B::edge(uv, vu));
    else {
      assert(hld_.parent(u) == v);
      seg_.set(hld_.index(u), B::edge(vu, uv));
    }
  }
  S vertices(int u, int v) { return fold(u, v, false); }
  S edges(int u, int v) { return fold(u, v, true); }
 private:
  const HeavyLightDecomposition& hld_;
  SegmentTree& seg_;
  S fold(int u, int v, bool edges) {
    S result = M::id();
    auto visit = [&](const HldSegment& s) {
      auto value = seg_.prod(s.l, s.r);
      result = M::op(result, s.reversed ? value.backward : value.forward);
    };
    if (edges) hld_.for_each_path_edges(u, v, visit);
    else hld_.for_each_path_vertices(u, v, visit);
    return result;
  }
};
template <class M, class SegmentTree>
HldPathFold<M, SegmentTree> make_hld_path_fold(const HeavyLightDecomposition& hld, SegmentTree& seg) {
  return {hld, seg};
}
template <class M, class SegmentTree>
HldPathFold<M, SegmentTree> make_hld_path_fold(const HeavyLightDecomposition&&, SegmentTree&) = delete;

}  // namespace cp
