#pragma once

#include <unordered_set>
#include <string>
#include <utility>

#include "utils/hash_utils.h"

using IntStringPairSet = std::unordered_set<std::pair<int, std::string>, PairHash>;
using IntPairSet = std::unordered_set<std::pair<int, int>, PairHash>;
using StringPairSet = std::unordered_set<std::pair<std::string, std::string>, PairHash>;

struct EvaluatorUtil {
  /**
   * @brief Converts a set of pairs of integers and strings to a set of pairs of strings.
   *
   * This method converts a set of pairs of integers and strings (original) to a set of pairs of strings.
   *
   * @param original A set of pairs of integers and strings to be converted.
   * @return A set of pairs of strings.
   */
  static StringPairSet ToStringPairSet(IntStringPairSet& original);

  /*!
   * @brief Converts a set of pairs of integers to a set of pairs of strings.
   *
   * This method converts a set of pairs of integers (original) to a set of pairs of strings.
   *
   * @param original A set of pairs of integers to be converted.
   * @return A set of pairs of strings.
   */
  static StringPairSet ToStringPairSet(IntPairSet& original);

  /*!
   * @brief Converts a set of integers to a set of strings.
   *
   * This method converts a set of integers (original) to a set of strings.
   *
   * @param original A set of integers to be converted.
   * @return A set of strings.
   */
  static std::unordered_set<std::string> ToStringSet(std::unordered_set<int>& original);
};
