#pragma once
#include <string>
#include <vector>

// Extracted from legacy cpptemplate; historical provenance not established.
// Apache-2.0 applies only to rrrriki's copyrightable human contributions.
template <typename T>
inline std::vector<std::vector<T> > rotate_matrix(const std::vector<std::vector<T> > &matrix) {
  int n = matrix.size();
  int m = matrix[0].size();
  std::vector<std::vector<T> > rotated(m, std::vector<T>(n));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      rotated[j][n - i - 1] = matrix[i][j];
    }
  }
  return rotated;
}
/// string行列の回転 @param matrix 行列 @return 回転した行列
inline std::vector<std::string> rotate_matrix(const std::vector<std::string> &matrix) {
  int n = matrix.size();
  int m = matrix[0].size();
  std::vector<std::string> rotated(m, std::string(n, ' '));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      rotated[j][n - i - 1] = matrix[i][j];
    }
  }
  return rotated;
}
