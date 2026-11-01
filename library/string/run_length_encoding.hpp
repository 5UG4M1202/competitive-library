#pragma once
#include <string>
#include <utility>
#include <vector>

// Legacy template implementation; its historical provenance is unconfirmed.
inline std::vector<std::pair<char, int>> rle(const std::string &s){std::vector<std::pair<char, int>> vec;if (s.empty()) return vec;int cnt = 1; for(int i = 1; i < (int)s.size(); i++) {if(s[i] != s[i-1]){vec.emplace_back(s[i-1], cnt); cnt = 0;} cnt++;} vec.emplace_back(s.back(), cnt);return vec;}
template <class T> inline std::vector<std::pair<T, int>> rle(const std::vector<T> &v) {std::vector<std::pair<T, int>> vec;if (v.empty()) return vec;int cnt = 1; for(int i = 1; i < (int)v.size(); i++) {if(v[i] != v[i-1]){vec.emplace_back(v[i-1], cnt); cnt = 0;} cnt++;} vec.emplace_back(v.back(), cnt);return vec;}
