#pragma once

#include <string>
#include <unordered_set>
#include <utility>

#include "utils/hash_utils.h"

/**
 * Each Such-that, Pattern clause will call PKB and return this type as a result.
 * @synonym_names: Is a pair of declaration synonym (i.e variable v, then "v" is a synonym name)
 * @synonym_values: Is the set of valid value pairs returned by PKB.
 *
 * We use the same format to represent unary constraint as well (for instance,
 * Uses(1, v) will only return the value value for "v".
 *
 * Unary constraint will just have the same value for both synonym_names and the
 * same values in the synonym_values.
 * ("v", "v") = {("foo", "foo"), ("bar", "bar")}
 */
struct Constraint {
  std::pair<std::string, std::string> synonym_names;
  std::unordered_set<std::pair<std::string, std::string>, PairHash> synonym_values;
};
