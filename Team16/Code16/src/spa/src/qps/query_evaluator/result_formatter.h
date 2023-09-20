#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "qps/query_evaluator/constraint.h"

namespace ResultFormatter {

// Functions to get back results which is a combination of
// all non-existing columns with the constraint table value
std::unordered_set<std::string> CombinationWithTable(
    const std::unordered_map<ColName, std::vector<Cell>>& non_existing_values,
    const Table& table, const std::vector<ColName>& order);
}  // namespace ResultFormatter
