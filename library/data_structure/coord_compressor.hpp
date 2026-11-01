#pragma once
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <tuple>
#include <unordered_map>
#include <vector>

// Extracted from legacy cpptemplate; historical provenance not established.
// Apache-2.0 applies only to rrrriki's copyrightable human contributions.
template <typename T>
struct CoordCompressor {
  std::unordered_map<T, int> compressed_map;          // 元の座標 -> 圧縮後の座標
  std::unordered_map<int, T> reverse_compressed_map;  // 圧縮後の座標 -> 元の座標

  /// @brief 座標圧縮を行う (可変長引数) @param vectors 複数のvectorを受け取り圧縮
  template <typename... Vectors>
  auto build(Vectors &...vectors) {
    std::vector<T> pool;
    (pool.insert(pool.end(), vectors.begin(), vectors.end()), ...);
    prepare_mapping(pool);                                // 座標 -> idx 作成
    return std::make_tuple(compress_vector(vectors)...);  // 圧縮して返す
  }
  /// @brief std::vector<std::vector<T>> の圧縮 (二次元vector)
  std::vector<std::vector<int>> build2D(const std::vector<std::vector<T>> &vectors) {
    std::vector<T> pool;
    for (const auto &v : vectors)
      pool.insert(pool.end(), v.begin(), v.end());
    prepare_mapping(pool);  // 座標 -> idx 作成
    std::vector<std::vector<int>> compressed(vectors.size());
    for (std::size_t i = 0; i < vectors.size(); ++i) compressed[i] = compress_vector(vectors[i]);
    return compressed;  // 圧縮して返す
  }

  /// @brief 値を圧縮する @param value 圧縮する値 @return 圧縮された値
  int compress(const T &value) const {
    assert(compressed_map.count(value));
    return compressed_map.at(value);
  }

  /// @brief 圧縮値を元に戻す @param idx 圧縮されたインデックス @return 元の値
  T decompress(int idx) const {
    assert(reverse_compressed_map.count(idx));
    return reverse_compressed_map.at(idx);
  }

  /// @brief vectorを圧縮する @param vec 圧縮するvector @return 圧縮されたvector
  std::vector<int> compress_vector(const std::vector<T> &vec) const {
    std::vector<int> compressedVec(vec.size());
    std::transform(vec.begin(), vec.end(), compressedVec.begin(),
              [&](const T &val) { return compress(val); });
    return compressedVec;
  }

  /// @brief vectorを元に戻す @param vec 圧縮されたvector @return 元の値に戻されたvector
  std::vector<T> decompress_vector(const std::vector<int> &vec) const {
    std::vector<T> decompressedVec(vec.size());
    std::transform(vec.begin(), vec.end(), decompressedVec.begin(),
              [&](int val) { return decompress(val); });
    return decompressedVec;
  }

 private:
  /// @brief 座標 -> 圧縮値テーブルを構築 @param pool 全要素を平坦化した配列 @note sort,unique
  void prepare_mapping(std::vector<T> &pool) {
    std::sort(pool.begin(), pool.end());
    pool.erase(std::unique(pool.begin(), pool.end()), pool.end());
    compressed_map.clear();
    reverse_compressed_map.clear();
    for (int i = 0; i < static_cast<int>(pool.size()); ++i) {
      compressed_map[pool[i]] = i;
      reverse_compressed_map[i] = pool[i];
    }
  }
};
