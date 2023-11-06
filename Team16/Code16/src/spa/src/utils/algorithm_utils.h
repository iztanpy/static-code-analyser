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

template<typename T>
bool is_subset(const std::unordered_set<T>& subset, const std::unordered_set<T>& set) {
  if (subset.size() > set.size()) {
    return false;  // A subset cannot be larger than the set it's supposed to be part of.
  }

  for (const auto& elem : subset) {
    if (set.find(elem) == set.end()) {
      return false;  // Element of the subset is not found in the set.
    }
  }

  return true;  //  All elements of subset were found in the set.
}

}  // namespace algorithm_utils
