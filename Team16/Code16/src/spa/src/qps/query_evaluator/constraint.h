#pragma once

#include <string>
#include <unordered_set>
#include <utility>

#include "utils/hash_utils.h"

struct Constraint {
  std::pair<std::string, std::string> synonym_names;
  std::unordered_set<std::pair<std::string, std::string>, PairHash> synonym_values;
};
