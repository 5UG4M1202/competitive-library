#pragma once

#include <atcoder/lazysegtree>
#include <atcoder/segtree>

#include <limits>
#include <numeric>
#include <optional>
#include <type_traits>
#include <vector>

// ACL adapters and reusable range aggregates/actions.
namespace cp {

template <class M>
using Segtree = atcoder::segtree<typename M::value_type, M::op, M::id>;

template <class A>
using LazySegtree = atcoder::lazy_segtree<typename A::value_type, A::op, A::id,
                                         typename A::action_type, A::mapping,
                                         A::composition, A::action_id>;

enum class AggregateKind { sum, minimum, maximum };

// Nonnegative values represented by an unsigned integer type (excluding bool).
template <class T>
struct Gcd {
  static_assert(std::is_integral_v<T> && std::is_unsigned_v<T> && !std::is_same_v<T, bool>);
  using value_type = T;
  static value_type op(value_type a, value_type b) { return std::gcd(a, b); }
  static value_type id() { return 0; }
};

enum class BitwiseKind { bit_and, bit_or, bit_xor };

// Unsigned integers make the bit width and the AND identity explicit.
template <class T, BitwiseKind Kind>
struct Bitwise {
  static_assert(std::is_integral_v<T> && std::is_unsigned_v<T> && !std::is_same_v<T, bool>);
  using value_type = T;
  static value_type op(value_type a, value_type b) {
    if constexpr (Kind == BitwiseKind::bit_and) return static_cast<T>(a & b);
    if constexpr (Kind == BitwiseKind::bit_or) return static_cast<T>(a | b);
    return static_cast<T>(a ^ b);
  }
  static value_type id() {
    if constexpr (Kind == BitwiseKind::bit_and) return std::numeric_limits<T>::max();
    return 0;
  }
};

// Empty ranges have count zero; the minimum itself is meaningful when count > 0.
template <class T>
struct MinCount {
  static_assert(std::is_integral_v<T> && !std::is_same_v<T, bool>);
  struct value_type {
    T value;
    long long count;
  };
  static value_type op(value_type a, value_type b) {
    if (a.count == 0) return b;
    if (b.count == 0) return a;
    if (a.value < b.value) return a;
    if (b.value < a.value) return b;
    return {a.value, a.count + b.count};
  }
  static value_type id() { return {std::numeric_limits<T>::max(), 0}; }
  static value_type leaf(T value) { return {value, 1}; }
};

template <class T>
struct RangeValue {
  T value;
  long long length;
};

// T must be a signed integer type. All intermediate arithmetic must fit.
template <class T, AggregateKind Kind>
struct RangeAggregate {
  static_assert(std::is_integral_v<T> && std::is_signed_v<T>);
  using value_type = RangeValue<T>;

  static value_type op(value_type a, value_type b) {
    if (a.length == 0) return b;
    if (b.length == 0) return a;
    if constexpr (Kind == AggregateKind::sum)
      return {static_cast<T>(a.value + b.value), a.length + b.length};
    if constexpr (Kind == AggregateKind::minimum)
      return {a.value < b.value ? a.value : b.value, a.length + b.length};
    return {a.value > b.value ? a.value : b.value, a.length + b.length};
  }

  static value_type id() {
    if constexpr (Kind == AggregateKind::minimum)
      return {std::numeric_limits<T>::max(), 0};
    if constexpr (Kind == AggregateKind::maximum)
      return {std::numeric_limits<T>::lowest(), 0};
    return {T(0), 0};
  }

  static value_type leaf(T value) { return {value, 1}; }
};

template <class T, AggregateKind Kind>
struct RangeAdd : RangeAggregate<T, Kind> {
  using value_type = typename RangeAggregate<T, Kind>::value_type;
  using action_type = T;
  static value_type mapping(action_type f, value_type x) {
    if (x.length == 0) return x;
    if constexpr (Kind == AggregateKind::sum) x.value += f * static_cast<T>(x.length);
    else x.value += f;
    return x;
  }
  static action_type composition(action_type f, action_type g) { return f + g; }
  static action_type action_id() { return T(0); }
};

template <class T, AggregateKind Kind>
struct RangeAssign : RangeAggregate<T, Kind> {
  using value_type = typename RangeAggregate<T, Kind>::value_type;
  using action_type = std::optional<T>;
  static value_type mapping(action_type f, value_type x) {
    if (!f || x.length == 0) return x;
    if constexpr (Kind == AggregateKind::sum) x.value = *f * static_cast<T>(x.length);
    else x.value = *f;
    return x;
  }
  static action_type composition(action_type f, action_type g) { return f ? f : g; }
  static action_type action_id() { return std::nullopt; }
};

// Apply x -> mul*x + add to each element; composition(f,g) means f after g.
template <class T>
struct RangeAffineSum : RangeAggregate<T, AggregateKind::sum> {
  using value_type = typename RangeAggregate<T, AggregateKind::sum>::value_type;
  struct action_type {
    T mul, add;
    bool operator==(const action_type&) const = default;
  };
  static value_type mapping(action_type f, value_type x) {
    return {static_cast<T>(f.mul * x.value + f.add * static_cast<T>(x.length)), x.length};
  }
  static action_type composition(action_type f, action_type g) {
    return {static_cast<T>(f.mul * g.mul), static_cast<T>(f.mul * g.add + f.add)};
  }
  static action_type action_id() { return {T(1), T(0)}; }
};

template <class A, class T>
std::vector<typename A::value_type> range_values(const std::vector<T>& values) {
  std::vector<typename A::value_type> result;
  result.reserve(values.size());
  for (auto value : values) result.push_back(A::leaf(value));
  return result;
}

}  // namespace cp
