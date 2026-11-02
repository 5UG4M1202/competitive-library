// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
// Intended correctness: path traversal itself does not allocate per segment.
#include "../../library/graph/heavy_light_decomposition.hpp"
#include "../../library/data_structure/monoid_segtree.hpp"

#include <cassert>
#include <cstdlib>
#include <new>
#include <vector>

static bool track_allocations = false;
static int allocations = 0;
void* operator new(std::size_t n) {
  if (track_allocations) ++allocations;
  if (void* p = std::malloc(n ? n : 1)) return p;
  throw std::bad_alloc();
}
void operator delete(void* p) noexcept { std::free(p); }
void operator delete(void* p, std::size_t) noexcept { std::free(p); }

struct Sum {
  using value_type = long long;
  static value_type op(value_type a, value_type b) { return a + b; }
  static value_type id() { return 0; }
};

int main() {
  constexpr int n = 127;
  std::vector<std::vector<int>> g(n);
  for (int v = 1; v < n; ++v) {
    int parent = (v - 1) / 2;
    g[parent].push_back(v); g[v].push_back(parent);
  }
  cp::HeavyLightDecomposition hld(g);
  using B = cp::BidirectionalMonoid<Sum>;
  cp::Segtree<B> seg(std::vector<B::value_type>(n, B::vertex(1)));
  auto fold = cp::make_hld_path_fold<Sum>(hld, seg);
  for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v) {
    int vertices = 0;
    allocations = 0; track_allocations = true;
    hld.for_each_path_vertices(u, v, [&](auto s) { vertices += s.r - s.l; });
    auto result = fold.vertices(u, v);
    track_allocations = false;
    assert(allocations == 0 && result == vertices);
    allocations = 0; track_allocations = true;
    auto segments = hld.path_vertices(u, v);
    track_allocations = false;
    assert(allocations == 1 && !segments.empty());
    allocations = 0; track_allocations = true;
    auto empty = hld.path_edges(u, u);
    track_allocations = false;
    assert(allocations == 0 && empty.empty());
  }
}
