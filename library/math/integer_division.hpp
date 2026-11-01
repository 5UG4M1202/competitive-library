#pragma once
#include <cassert>
#include <limits>

// Legacy template functions; historical provenance is unconfirmed.
// Previous out_div closely matches https://blog.dnek.net/entry/2020/12/06/011925 ; reuse terms unconfirmed.
inline constexpr long long ceil_div(const long long a, const long long b) {assert(b != 0 && !(a == std::numeric_limits<long long>::min() && b == -1)); long long q = a / b, r = a % b; return q + (r != 0 && ((a < 0) == (b < 0)));}
inline constexpr long long floor_div(const long long a, const long long b) {assert(b != 0 && !(a == std::numeric_limits<long long>::min() && b == -1)); long long q = a / b, r = a % b; return q - (r != 0 && ((a < 0) != (b < 0)));}
inline constexpr long long out_div(long long x, long long y) {assert(y != 0 && !(x == std::numeric_limits<long long>::min() && y == -1)); long long q = x / y, r = x % y; return r == 0 ? q : q + (((x < 0) == (y < 0)) ? 1 : -1);}
