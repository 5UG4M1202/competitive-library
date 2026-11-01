#pragma once

#include <utility>
#include <vector>

// Adapted from NyaanNyaan/library, CC0-1.0:
// https://github.com/NyaanNyaan/library/blob/b3981adc80a800b2584980b01821324ea6c77183/data-structure/union-find-with-potential.hpp
// Local changes: standard includes and std:: qualification.
template <class T>
struct UnionFindWithPotential {
  std::vector<int> dat;
  std::vector<T> pot;

  UnionFindWithPotential(int N) : dat(N, -1), pot(N, T()) {}

  int root(int x) {
    if (dat[x] < 0) return x;
    int r = root(dat[x]);
    pot[x] += pot[dat[x]];
    return dat[x] = r;
  }

  T potential(int x) {
    root(x);
    return pot[x];
  }

  bool same(int x, int y) { return root(x) == root(y); }

  T diff(int x, int y) { return potential(x) - potential(y); }

  bool merge(int x, int y, T p) {
    p += potential(y) - potential(x);
    x = root(x), y = root(y);
    if (x == y) return p == T();
    if (dat[x] < dat[y]) std::swap(x, y), p = -p;
    dat[y] += dat[x];
    dat[x] = y;
    pot[x] = p;
    return true;
  }

  int size(int x) { return -dat[root(x)]; }
};
