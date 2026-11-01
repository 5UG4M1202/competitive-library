#pragma once
#include <algorithm>
#include <string>
#include <vector>

// Extracted from legacy cpptemplate; historical provenance not established.
// Apache-2.0 applies only to rrrriki's copyrightable human contributions.
inline int levenstein_distance(const std::string &s1, const std::string &s2) {
  int n = s1.size();
  int m = s2.size();
  std::vector<std::vector<int> > dp(n + 1, std::vector<int>(m + 1, 0));
  for (int i = 0; i <= n; i++) dp[i][0] = i;
  for (int j = 0; j <= m; j++) dp[0][j] = j;
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= m; j++) {
      dp[i][j] = std::min({dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + (s1[i - 1] != s2[j - 1])});
    }
  }
  return dp[n][m];
}
