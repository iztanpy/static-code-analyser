#pragma once

#include <unordered_set>

namespace algorithm_utils {
template<typename T>
std::unordered_set<T> intersect(const std::unordered_set<T>& set1, const std::unordered_set<T>& set2) {
  std::unordered_set<T> result;

  // Assume set1 is smaller or equal to set2 for efficient intersection
  if (set1.size() > set2.size()) {
    return intersect(set2, set1);
  }

  for (const auto& item : set1) {
    if (set2.find(item) != set2.end()) {
      result.insert(item);
    }
  }

  return result;
}
}  // namespace algorithm_utils
