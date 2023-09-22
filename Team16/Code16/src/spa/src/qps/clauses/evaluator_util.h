#pragma once

#include <unordered_set>
#include <string>

#include "utils/hash_utils.h"

using IntStringPairSet = std::unordered_set<std::pair<int, std::string>, PairHash>;
using IntPairSet = std::unordered_set<std::pair<int, int>, PairHash>;
using StringPairSet = std::unordered_set<std::pair<std::string, std::string>, PairHash>;

struct EvaluatorUtil {
  // TODO(phuccuongngo99): Move these to .cpp file please
  static StringPairSet ToStringPairSet(IntStringPairSet& original) {
    StringPairSet converted;
    for (const auto& entry : original) {
      std::string first_str = std::to_string(entry.first);
      converted.insert({first_str, entry.second});
    }
    return converted;
  }

  static StringPairSet ToStringPairSet(IntPairSet& original) {
    StringPairSet converted;
    for (const auto& entry : original) {
      converted.insert({std::to_string(entry.first), std::to_string(entry.second)});
    }
    return converted;
  }

  static std::unordered_set<std::string> ToStringSet(std::unordered_set<int>& original) {
    std::unordered_set<std::string> converted;
    for (const auto& entry : original) {
      converted.insert(std::to_string(entry));
    }
    return converted;
  }
};
