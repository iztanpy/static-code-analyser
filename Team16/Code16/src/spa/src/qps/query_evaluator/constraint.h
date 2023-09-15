#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <vector>

#include "utils/hash_utils.h"

// Let's say users define "variable v0, v1; assign a1"
// Then in our constraint table, the column name will be "v0", "v1", and "a1"
using ColName = std::string;

// Cell is the value that a Column name can take
// We just cast everything (variable names, procedure names, int values of statement
// number to std::string as of now
using Cell = std::string;

// A table is just a map from column name to column
using Table = std::unordered_map<ColName, std::vector<Cell>>;

// Represents the Binary constraint of two synonyms. This
// is all possible valid pair of values that satisfy a clause
struct BinaryConstraint {
  std::pair<ColName, ColName> pair_col_names;
  std::unordered_set<std::pair<Cell, Cell>, PairHash> pair_values;
};

struct UnaryConstraint {
  ColName col_name;
  std::unordered_set<Cell> values;
};

using Constraint = std::variant<BinaryConstraint, UnaryConstraint>;

