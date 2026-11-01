#pragma once
#include <algorithm>
#include <cassert>
#include <limits>
#include <map>
#include <vector>

// Legacy template functions; historical provenance is unconfirmed.
inline bool is_prime(long long x){if(x<2)return false;for (long long i=2; i<=x/i; i++){if(x%i==0)return false;}return true;}
inline std::map<long long,int> prime_factor(long long n) {assert(n >= 1); std::map<long long,int> ret; for(long long i=2; i<=n/i; i++) {while(n%i == 0) {ret[i]++; n /= i;}} if(n != 1) ret[n]=1;return ret;}
inline std::vector<bool> sieve_of_era(long long n) {assert(0 <= n && n < std::numeric_limits<long long>::max()); std::vector<bool> ret(n+1,true); ret[0]=false; if(n>=1) ret[1]=false; for(long long i=2; i<=n/i; i++) {if(ret[i]) {for(long long j=i*i; j<=n;) {ret[j]=false; if(j>n-i) break; j+=i;}}} return ret;}
inline std::vector<long long> divisor(long long n) {assert(n >= 1); std::vector<long long> ret; for(long long i=1; i<=n/i; i++) {if(n%i == 0) {ret.push_back(i); if(i != n/i) ret.emplace_back(n/i);}} std::sort(ret.begin(), ret.end()); return ret;}
