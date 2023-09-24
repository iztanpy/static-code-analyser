#pragma once

#include <unordered_set>
#include <string>
#include <utility>

#include "utils/hash_utils.h"

using IntStringPairSet = std::unordered_set<std::pair<int, std::string>, PairHash>;
using IntPairSet = std::unordered_set<std::pair<int, int>, PairHash>;
using StringPairSet = std::unordered_set<std::pair<std::string, std::string>, PairHash>;

struct EvaluatorUtil {
  // TODO(phuccuongngo99): Move these to .cpp file please
  static StringPairSet ToStringPairSet(IntStringPairSet& original);

  static StringPairSet ToStringPairSet(IntPairSet& original);

  static std::unordered_set<std::string> ToStringSet(std::unordered_set<int>& original);
};
