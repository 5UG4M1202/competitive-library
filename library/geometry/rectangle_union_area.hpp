#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <vector>

namespace cp::geometry {

struct IntegerRectangle { std::int64_t xl, yl, xr, yr; };

// Area of half-open rectangles; degenerate rectangles contribute zero.
// Throws if the nonnegative area is not representable as signed __int128.
inline __int128_t rectangle_union_area(const std::vector<IntegerRectangle>& rectangles) {
  using Wide = __int128_t;
  struct Event { std::int64_t x, lo, hi; int delta; };
  if (rectangles.size() > static_cast<std::size_t>(std::numeric_limits<int>::max()))
    throw std::length_error("too many rectangles");
  std::vector<Event> events;
  std::vector<std::int64_t> ys;
  for (const auto& r : rectangles) {
    assert(r.xl <= r.xr && r.yl <= r.yr);
    if (r.xl == r.xr || r.yl == r.yr) continue;
    events.push_back({r.xl,r.yl,r.yr,1});
    events.push_back({r.xr,r.yl,r.yr,-1});
    ys.push_back(r.yl); ys.push_back(r.yr);
  }
  if (events.empty()) return 0;
  std::sort(ys.begin(),ys.end());
  ys.erase(std::unique(ys.begin(),ys.end()),ys.end());
  std::sort(events.begin(),events.end(),[](const Event& a,const Event& b){return a.x < b.x;});
  const std::size_t count = ys.size()-1;
  if (count > (std::numeric_limits<std::size_t>::max()-4)/4) throw std::length_error("too many y coordinates");
  std::vector<int> cover(count*4+4,0);
  std::vector<Wide> length(count*4+4,0);
  const auto update = [&](auto&& self,std::size_t node,std::size_t l,std::size_t r,std::size_t ql,std::size_t qr,int delta) -> void {
    if (qr <= l || r <= ql) return;
    if (ql <= l && r <= qr) cover[node] += delta;
    else {
      const auto m=l+(r-l)/2;
      self(self,node*2,l,m,ql,qr,delta);
      self(self,node*2+1,m,r,ql,qr,delta);
    }
    assert(cover[node]>=0);
    if (cover[node] > 0) length[node] = Wide(ys[r])-Wide(ys[l]);
    else if (r-l == 1) length[node] = 0;
    else length[node] = length[node*2] + length[node*2+1];
  };
  Wide area = 0;
  auto previous_x = events.front().x;
  for (std::size_t i=0;i<events.size();) {
    const auto x=events[i].x;
    const Wide dx=Wide(x)-Wide(previous_x);
    Wide strip, next;
    if (__builtin_mul_overflow(dx,length[1],&strip) || __builtin_add_overflow(area,strip,&next))
      throw std::overflow_error("rectangle union area overflow");
    area=next;
    do {
      const auto& e=events[i];
      const auto l=static_cast<std::size_t>(std::lower_bound(ys.begin(),ys.end(),e.lo)-ys.begin());
      const auto r=static_cast<std::size_t>(std::lower_bound(ys.begin(),ys.end(),e.hi)-ys.begin());
      update(update,1,0,count,l,r,e.delta);
      ++i;
    } while (i<events.size() && events[i].x==x);
    previous_x=x;
  }
  return area;
}
}  // namespace cp::geometry
