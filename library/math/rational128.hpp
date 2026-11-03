#pragma once

#include <algorithm>
#include <compare>
#include <concepts>
#include <cstdint>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace cp {

// Finite, reduced rationals with a signed 128-bit numerator and positive denominator.
class Rational128 {
 public:
  using integer_type = __int128_t;
  using unsigned_type = __uint128_t;

  Rational128(integer_type numerator = 0, integer_type denominator = 1) {
    if (denominator == 0) throw std::domain_error("zero denominator");
    if (numerator == 0) return;
    auto n = magnitude(numerator), d = magnitude(denominator);
    const auto g = gcd(n,d);
    n /= g; d /= g;
    if (d > max_positive()) throw std::overflow_error("denominator out of range");
    num_ = signed_value(n, (numerator < 0) != (denominator < 0));
    den_ = static_cast<integer_type>(d);
  }
  template <std::floating_point F> Rational128(F) = delete;
  template <std::floating_point F, class T> Rational128(F,T) = delete;
  template <class T, std::floating_point F> Rational128(T,F) = delete;

  integer_type numerator() const { return num_; }
  integer_type denominator() const { return den_; }
  integer_type trunc() const { return num_ / den_; }
  integer_type floor() const { const auto q = num_ / den_; return q - (num_ % den_ < 0); }
  integer_type ceil() const { const auto q = num_ / den_; return q + (num_ % den_ > 0); }
  int sign() const { return (num_ > 0) - (num_ < 0); }
  Rational128 reciprocal() const { return Rational128(1) / *this; }
  explicit operator long double() const { return static_cast<long double>(num_) / static_cast<long double>(den_); }

  friend bool operator==(const Rational128& a, const Rational128& b) {
    return a.num_ == b.num_ && a.den_ == b.den_;
  }
  friend std::strong_ordering operator<=>(const Rational128& a, const Rational128& b) {
    if (a.num_ < 0 && b.num_ >= 0) return std::strong_ordering::less;
    if (a.num_ >= 0 && b.num_ < 0) return std::strong_ordering::greater;
    int result = compare_nonnegative(magnitude(a.num_), static_cast<unsigned_type>(a.den_),
                                     magnitude(b.num_), static_cast<unsigned_type>(b.den_));
    if (a.num_ < 0) result = -result;
    return result < 0 ? std::strong_ordering::less : result > 0 ? std::strong_ordering::greater : std::strong_ordering::equal;
  }

  Rational128 operator-() const {
    return from_parts(signed_value(magnitude(num_), num_ >= 0), den_);
  }
  friend Rational128 operator+(const Rational128& a, const Rational128& b) { return add(a,b,false); }
  friend Rational128 operator-(const Rational128& a, const Rational128& b) { return add(a,b,true); }
  friend Rational128 operator*(const Rational128& a, const Rational128& b) {
    auto an = magnitude(a.num_), bn = magnitude(b.num_);
    auto ad = static_cast<unsigned_type>(a.den_), bd = static_cast<unsigned_type>(b.den_);
    auto g = gcd(an,bd); an /= g; bd /= g;
    g = gcd(bn,ad); bn /= g; ad /= g;
    const bool negative = (a.num_ < 0) != (b.num_ < 0);
    const auto n = multiply(an,bn,negative ? min_magnitude() : max_positive());
    const auto d = multiply(ad,bd,max_positive());
    return from_parts(signed_value(n,negative), static_cast<integer_type>(d));
  }
  friend Rational128 operator/(const Rational128& a, const Rational128& b) {
    if (b.num_ == 0) throw std::domain_error("division by zero");
    if (a.num_ == 0) return {};
    auto an = magnitude(a.num_), bn = magnitude(b.num_);
    auto ad = static_cast<unsigned_type>(a.den_), bd = static_cast<unsigned_type>(b.den_);
    auto g = gcd(an,bn); an /= g; bn /= g;
    g = gcd(bd,ad); bd /= g; ad /= g;
    const bool negative = (a.num_ < 0) != (b.num_ < 0);
    const auto n = multiply(an,bd,negative ? min_magnitude() : max_positive());
    const auto d = multiply(ad,bn,max_positive());
    return from_parts(signed_value(n,negative),static_cast<integer_type>(d));
  }
  Rational128& operator+=(const Rational128& b) { return *this = *this + b; }
  Rational128& operator-=(const Rational128& b) { return *this = *this - b; }
  Rational128& operator*=(const Rational128& b) { return *this = *this * b; }
  Rational128& operator/=(const Rational128& b) { return *this = *this / b; }

  std::string str() const {
    auto result = integer_string(num_);
    if (den_ != 1) result += "/" + integer_string(den_);
    return result;
  }
  static Rational128 parse(std::string_view text) {
    const auto slash = text.find('/');
    const auto n = parse_integer(text.substr(0,slash));
    const auto d = slash == text.npos ? integer_type{1} : parse_integer(text.substr(slash+1));
    return Rational128(n,d);
  }
  friend std::ostream& operator<<(std::ostream& out, const Rational128& value) { return out << value.str(); }
  friend std::istream& operator>>(std::istream& in, Rational128& value) {
    std::string token;
    if (!(in >> token)) return in;
    try { value = parse(token); }
    catch (const std::invalid_argument&) { in.setstate(std::ios::failbit); }
    catch (const std::domain_error&) { in.setstate(std::ios::failbit); }
    catch (const std::overflow_error&) { in.setstate(std::ios::failbit); }
    return in;
  }

 private:
  integer_type num_ = 0, den_ = 1;
  static constexpr unsigned_type min_magnitude() { return unsigned_type{1} << 127; }
  static constexpr unsigned_type max_positive() { return min_magnitude() - 1; }
  static unsigned_type magnitude(integer_type x) {
    const auto u = static_cast<unsigned_type>(x);
    return x < 0 ? unsigned_type{0} - u : u;
  }
  static unsigned_type gcd(unsigned_type a, unsigned_type b) {
    while (b != 0) { const auto r = a % b; a = b; b = r; }
    return a;
  }
  static integer_type signed_value(unsigned_type value, bool negative) {
    if (value > (negative ? min_magnitude() : max_positive())) throw std::overflow_error("numerator out of range");
    if (!negative || value == 0) return static_cast<integer_type>(value);
    return -static_cast<integer_type>(value-1)-1;
  }
  static unsigned_type multiply(unsigned_type a, unsigned_type b, unsigned_type limit) {
    if (a != 0 && b > limit/a) throw std::overflow_error("rational multiplication overflow");
    return a*b;
  }
  static Rational128 from_parts(integer_type n, integer_type d) {
    Rational128 result; result.num_ = n; result.den_ = d; return result;
  }
  static Rational128 add(const Rational128& a, const Rational128& b, bool subtract) {
    const auto g = gcd(static_cast<unsigned_type>(a.den_),static_cast<unsigned_type>(b.den_));
    const auto ad = a.den_ / static_cast<integer_type>(g), bd = b.den_ / static_cast<integer_type>(g);
    integer_type x, y, sum;
    if (__builtin_mul_overflow(a.num_,bd,&x) || __builtin_mul_overflow(b.num_,ad,&y) ||
        (subtract ? __builtin_sub_overflow(x,y,&sum) : __builtin_add_overflow(x,y,&sum)))
      throw std::overflow_error("rational addition overflow");
    if (sum == 0) return {};
    const auto h = gcd(magnitude(sum),g);
    const auto den = multiply(static_cast<unsigned_type>(ad), static_cast<unsigned_type>(b.den_) / h, max_positive());
    const auto num = signed_value(magnitude(sum) / h, sum < 0);
    return from_parts(num,static_cast<integer_type>(den));
  }
  // Continued-fraction comparison avoids overflowing cross-products.
  static int compare_nonnegative(unsigned_type a,unsigned_type b,unsigned_type c,unsigned_type d) {
    bool reversed = false;
    for (;;) {
      const auto q1 = a/b, q2 = c/d;
      if (q1 != q2) return (q1 < q2) != reversed ? -1 : 1;
      const auto r1 = a%b, r2 = c%d;
      if (r1 == 0 || r2 == 0) {
        if (r1 == r2) return 0;
        return (r1 == 0) != reversed ? -1 : 1;
      }
      a=b; b=r1; c=d; d=r2; reversed=!reversed;
    }
  }
  static integer_type parse_integer(std::string_view s) {
    if (s.empty()) throw std::invalid_argument("empty integer");
    bool negative = false;
    if (s.front() == '-' || s.front() == '+') { negative = s.front() == '-'; s.remove_prefix(1); }
    if (s.empty()) throw std::invalid_argument("missing digits");
    const auto limit = negative ? min_magnitude() : max_positive();
    unsigned_type value = 0;
    for (char c : s) {
      if (c < '0' || c > '9') throw std::invalid_argument("invalid digit");
      const auto digit = static_cast<unsigned>(c-'0');
      if (value > (limit-digit)/10) throw std::overflow_error("integer out of range");
      value = value*10 + digit;
    }
    return signed_value(value,negative);
  }
  static std::string integer_string(integer_type value) {
    auto n = magnitude(value);
    std::string result;
    do { result.push_back(static_cast<char>('0' + n%10)); n /= 10; } while (n);
    if (value < 0) result.push_back('-');
    std::reverse(result.begin(),result.end());
    return result;
  }
};
}  // namespace cp
