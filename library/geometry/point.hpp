#pragma once
#include <array>
#include <cassert>
#include <cstddef>
#include <type_traits>

namespace cp::geometry {
template <class T, std::size_t N> struct Point {
  static_assert(N > 0);
  std::array<T, N> coordinates{};
  T& operator[](std::size_t i) { return coordinates[i]; }
  const T& operator[](std::size_t i) const { return coordinates[i]; }
  friend bool operator==(const Point&, const Point&) = default;
  friend bool operator<(const Point& a, const Point& b) { return a.coordinates < b.coordinates; }
};
template <class T> struct Point<T, 2> {
  T x{}, y{};
  T& operator[](std::size_t i) { assert(i < 2); return i == 0 ? x : y; }
  const T& operator[](std::size_t i) const { assert(i < 2); return i == 0 ? x : y; }
  friend bool operator==(const Point&, const Point&) = default;
  friend bool operator<(const Point& a, const Point& b) {
    return a.x < b.x || (a.x == b.x && a.y < b.y);
  }
};
template <class T, std::size_t N> Point<T,N> operator+(Point<T,N> a, const Point<T,N>& b) {
  for (std::size_t i=0;i<N;++i) a[i] += b[i];
  return a;
}
template <class T, std::size_t N> Point<T,N> operator-(Point<T,N> a, const Point<T,N>& b) {
  for (std::size_t i=0;i<N;++i) a[i] -= b[i];
  return a;
}
template <class T, std::size_t N> Point<T,N> operator-(Point<T,N> a) {
  for (std::size_t i=0;i<N;++i) a[i] = -a[i];
  return a;
}
// Wide must accommodate every product and partial sum; conversion precedes subtraction.
template <class Wide, class T, std::size_t N> Wide dot_n(const Point<T,N>& a, const Point<T,N>& b) {
  Wide sum{}; for (std::size_t i=0;i<N;++i) sum += Wide(a[i]) * Wide(b[i]); return sum;
}
template <class Wide, class T, std::size_t N> Wide squared_distance_n(const Point<T,N>& a, const Point<T,N>& b) {
  Wide sum{}; for (std::size_t i=0;i<N;++i) { Wide d = Wide(a[i]) - Wide(b[i]); sum += d*d; } return sum;
}
template <class Wide, class T> Wide cross2(const Point<T,2>& a, const Point<T,2>& b) {
  return Wide(a.x)*Wide(b.y) - Wide(a.y)*Wide(b.x);
}
template <class Wide, class T> Point<Wide,3> cross3(const Point<T,3>& a, const Point<T,3>& b) {
  return {{Wide(a[1])*Wide(b[2])-Wide(a[2])*Wide(b[1]),
           Wide(a[2])*Wide(b[0])-Wide(a[0])*Wide(b[2]),
           Wide(a[0])*Wide(b[1])-Wide(a[1])*Wide(b[0])}};
}
}
