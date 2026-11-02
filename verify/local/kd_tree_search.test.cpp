// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/geometry/integer_kd_tree.hpp"
#include "../../library/geometry/real_kd_tree.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
#include <random>
#include <vector>
using namespace cp::geometry;
struct Sum {
  using value_type = long long;
  static value_type id() { return 0; }
  static value_type op(value_type a, value_type b) { return a+b; }
};
int main() {
  IntegerKdTree<Sum> empty({},{});
  RealKdTree<Sum> empty_real({},{});
  assert(!empty.nearest({0,0}) && !empty_real.nearest({0,0}));
  int calls = 0;
  empty.for_each_in_rect(-1,1,-1,1,[&](auto) { ++calls; });
  assert(calls == 0);
  const auto max = std::numeric_limits<std::int64_t>::max();
  IntegerKdTree<Sum> edge({{max,max},{max,max}}, {2,3});
  std::vector<std::size_t> out;
  edge.for_each_in_rect(IntegerWide(max),IntegerWide(max)+1,IntegerWide(max),IntegerWide(max)+1,
                        [&](auto i) { out.push_back(i); });
  std::sort(out.begin(),out.end());
  assert((out == std::vector<std::size_t>{0,1}));
  assert(edge.nearest({max,max})->distance2 == 0);
  std::mt19937 rng(30219);
  for (int trial = 0; trial < 300; ++trial) {
    int n = static_cast<int>(rng()%80);
    std::vector<IntegerPoint> ip;
    std::vector<RealPoint> rp;
    std::vector<long long> values;
    for (int i=0;i<n;++i) {
      const int x = static_cast<int>(rng()%21)-10, y = static_cast<int>(rng()%21)-10;
      ip.push_back({x,y}); rp.push_back({static_cast<long double>(x)/2,static_cast<long double>(y)/2});
      values.push_back(i+1);
    }
    IntegerKdTree<Sum> ik(ip,values);
    RealKdTree<Sum> rk(rp,values);
    for (int step=0;step<120;++step) {
      int x = static_cast<int>(rng()%31)-15, y = static_cast<int>(rng()%31)-15;
      int xr = static_cast<int>(rng()%31)-15, yr = static_cast<int>(rng()%31)-15;
      std::vector<std::size_t> expected, a, b;
      for (int i=0;i<n;++i)
        if (x<=ip[i].x && ip[i].x<xr && y<=ip[i].y && ip[i].y<yr) expected.push_back(static_cast<std::size_t>(i));
      ik.for_each_in_rect(x,xr,y,yr,[&](auto id){ a.push_back(id); });
      rk.for_each_in_rect(x/2.0L,xr/2.0L,y/2.0L,yr/2.0L,[&](auto id){ b.push_back(id); });
      std::sort(a.begin(),a.end()); std::sort(b.begin(),b.end());
      assert(a == expected && b == expected);
      auto ni = ik.nearest({x,y}); auto nr = rk.nearest({x/2.0L,y/2.0L});
      assert(bool(ni) == (n>0) && bool(nr) == (n>0));
      if (n) {
        IntegerWide best = squared_distance(ip[0],{x,y});
        for (auto p : ip) best = std::min(best,squared_distance(p,{x,y}));
        assert(ni->index < ip.size() && ni->distance2 == best);
        assert(squared_distance(ip[ni->index],{x,y}) == best);
        assert(nr->index < rp.size() && nr->distance2 == static_cast<long double>(best)/4);
        const auto p=rp[nr->index];
        assert((p.x-x/2.0L)*(p.x-x/2.0L)+(p.y-y/2.0L)*(p.y-y/2.0L) == nr->distance2);
      }
      if (n && step%17 == 0) {
        const auto id = rng()%static_cast<unsigned>(n);
        ik.set(id,123); rk.set(id,123);
      }
    }
  }
  const auto adjacent = std::nextafter(1.0L,std::numeric_limits<long double>::infinity());
  RealKdTree<Sum> precision({{1,0},{adjacent,0}}, {1,1});
  std::vector<std::size_t> exact_ids;
  precision.for_each_in_rect(1,adjacent,-1,1,[&](auto id) { exact_ids.push_back(id); });
  assert((exact_ids == std::vector<std::size_t>{0}));
  for (int trial=0;trial<80;++trial) {
    std::vector<RealPoint> points;
    for (int i=0;i<60;++i)
      points.push_back({(static_cast<int>(rng()%2001)-1000)/13.0L,
                        (static_cast<int>(rng()%2001)-1000)/17.0L});
    RealKdTree<Sum> tree(points,std::vector<long long>(points.size(),1));
    for (int j=0;j<100;++j) {
      RealPoint q{(static_cast<int>(rng()%2001)-1000)/19.0L,
                  (static_cast<int>(rng()%2001)-1000)/23.0L};
      auto nearest = tree.nearest(q);
      auto dist = [&](RealPoint p) { auto dx=p.x-q.x, dy=p.y-q.y; return dx*dx+dy*dy; };
      long double best = dist(points[0]);
      for (auto p : points) best = std::min(best,dist(p));
      assert(nearest && nearest->index < points.size());
      assert(nearest->distance2 == best && dist(points[nearest->index]) == best);
    }
  }
}
