// competitive-verifier: STANDALONE
// competitive-verifier: DISPLAY no-index
#include "../../library/math/rational128.hpp"
#include <boost/multiprecision/cpp_int.hpp>
#include <cassert>
#include <cstdint>
#include <limits>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
using cp::Rational128;
using I = Rational128::integer_type;
using U = Rational128::unsigned_type;
using boost::multiprecision::cpp_int;

cpp_int big(I x) {
  const U bits = static_cast<U>(x);
  U mag = x < 0 ? U(0)-bits : bits;
  cpp_int result = static_cast<std::uint64_t>(mag >> 64);
  result <<= 64;
  result += static_cast<std::uint64_t>(mag);
  return x < 0 ? -result : result;
}
void matches(const Rational128& x, cpp_int n, cpp_int d) {
  assert(big(x.numerator()) * d == n * big(x.denominator()));
  assert(x.denominator() > 0);
}
template <class Exception, class F> void throws(F f) {
  bool raised = false;
  try { f(); } catch (const Exception&) { raised = true; }
  assert(raised);
}
int main() {
  static_assert(!std::is_constructible_v<Rational128,double>);
  static_assert(!std::is_constructible_v<Rational128,int,double>);
  const I max = static_cast<I>((U(1)<<127)-1), min = -max-1;
  assert(Rational128(0,min) == Rational128());
  assert(Rational128(min,min) == Rational128(1));
  assert(Rational128(min,2).numerator() == min/2);
  throws<std::domain_error>([]{ Rational128 x(1,0); (void)x; });
  throws<std::overflow_error>([&]{ Rational128 x(1,min); (void)x; });
  throws<std::overflow_error>([&]{ auto x=-Rational128(min); (void)x; });
  assert(Rational128(max,max-1) > Rational128(max-1,max));
  assert(Rational128(min,max) < Rational128(-1));
  assert(Rational128(max,2) * Rational128(2,max) == Rational128(1));
  assert(Rational128(min,1) / Rational128(min,1) == Rational128(1));
  assert(Rational128(-7,3).trunc() == -2 && Rational128(-7,3).floor() == -3 && Rational128(-7,3).ceil() == -2);
  assert(Rational128(7,3).floor() == 2 && Rational128(7,3).ceil() == 3);
  assert(Rational128(min).floor() == min && Rational128(max).ceil() == max);
  assert(Rational128(-4,3).sign() == -1 && Rational128().sign() == 0 && Rational128(2).sign() == 1);
  assert(Rational128(-4,3).reciprocal() == Rational128(-3,4));
  throws<std::domain_error>([]{ (void)Rational128().reciprocal(); });
  throws<std::domain_error>([]{ auto x=Rational128(2)/Rational128(); (void)x; });
  throws<std::overflow_error>([&]{ auto x=Rational128(max)+Rational128(1); (void)x; });
  throws<std::overflow_error>([&]{ auto x=Rational128(max)*Rational128(2); (void)x; });
  Rational128 still(max,2);
  throws<std::overflow_error>([&]{ still += Rational128(max,2); });
  assert(still == Rational128(max,2)); // Mathematically representable, but intermediate sum is not.
  assert(Rational128::parse("-6/-8").str() == "3/4");
  assert(Rational128::parse("-170141183460469231731687303715884105728").numerator() == min);
  assert(Rational128(min).str() == "-170141183460469231731687303715884105728");
  throws<std::invalid_argument>([]{ (void)Rational128::parse("1/2/3"); });
  throws<std::overflow_error>([]{ (void)Rational128::parse("170141183460469231731687303715884105728"); });
  Rational128 unchanged(3,4);
  std::istringstream bad("1/0"); bad >> unchanged;
  assert(bad.fail() && unchanged == Rational128(3,4));
  std::stringstream stream; stream << Rational128(-6,8) << ' ' << Rational128(7);
  Rational128 a,b; stream >> a >> b;
  assert(a == Rational128(-3,4) && b == Rational128(7));
  std::mt19937_64 rng(112233);
  for (int i=0;i<30000;++i) {
    const auto rn = [&]{ return static_cast<long long>(rng()%2000000001ULL)-1000000000LL; };
    const auto rd = [&]{ return static_cast<long long>(rng()%1000000000ULL)+1; };
    I an=rn(), ad=rd(), bn=rn(), bd=rd();
    Rational128 x(an,ad), y(bn,bd);
    const cpp_int an_big=big(an), ad_big=big(ad), bn_big=big(bn), bd_big=big(bd);
    assert((x<y) == (an_big*bd_big < bn_big*ad_big));
    assert((x==y) == (an_big*bd_big == bn_big*ad_big));
    matches(x+y,an_big*bd_big+bn_big*ad_big,ad_big*bd_big);
    matches(x-y,an_big*bd_big-bn_big*ad_big,ad_big*bd_big);
    matches(x*y,an_big*bn_big,ad_big*bd_big);
    if (bn != 0) matches(x/y,an_big*bd_big,ad_big*bn_big);
    Rational128 c=x; c+=y; assert(c==x+y); c=x; c-=y; assert(c==x-y);
    c=x; c*=y; assert(c==x*y); if (bn) { c=x; c/=y; assert(c==x/y); }
  }
  for (int i=0;i<10000;++i) {
    auto random_positive = [&]() -> I {
      U val=(U(rng())<<63) | (U(rng())>>1);
      return static_cast<I>(val & ((U(1)<<127)-1));
    };
    I an=random_positive(), bn=random_positive(), ad=random_positive(), bd=random_positive();
    if (!ad) ad=1;
    if (!bd) bd=1;
    if (rng()&1) an=-an;
    if (rng()&1) bn=-bn;
    Rational128 x(an,ad),y(bn,bd);
    const cpp_int left=big(an)*big(bd), right=big(bn)*big(ad);
    assert((x<y)==(left<right));
    assert((x==y)==(left==right));
  }
}
