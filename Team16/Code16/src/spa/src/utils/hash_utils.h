#pragma once

#include <utility>

/*!
 * A hash function for pairs
 */
// ai-gen start (gpt 3.5, 0)
struct PairHash {
  template<typename T1, typename T2>
  std::size_t operator()(const std::pair<T1, T2>& p) const {
    // Get individual hashes
    std::size_t hash1 = std::hash<T1>{}(p.first);
    std::size_t hash2 = std::hash<T2>{}(p.second);

    // Inspired by Boost's hash_combine
    return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2));
  }
};
// ai-gen end
