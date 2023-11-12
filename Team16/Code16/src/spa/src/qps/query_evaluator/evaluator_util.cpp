#include "qps/query_evaluator/evaluator_util.h"

StringPairSet EvaluatorUtil::ToStringPairSet(IntStringPairSet& original) {
  StringPairSet converted;
  for (const auto& entry : original) {
    std::string first_str = std::to_string(entry.first);
    converted.insert({first_str, entry.second});
  }
  return converted;
}

StringPairSet EvaluatorUtil::ToStringPairSet(IntPairSet& original) {
  StringPairSet converted;
  for (const auto& entry : original) {
    converted.insert({std::to_string(entry.first), std::to_string(entry.second)});
  }
  return converted;
}

std::unordered_set<std::string> EvaluatorUtil::ToStringSet(std::unordered_set<int>& original) {
  std::unordered_set<std::string> converted;
  converted.reserve(original.size());
  for (const auto& entry : original) {
    converted.emplace(std::to_string(entry));
  }
  return converted;
}
