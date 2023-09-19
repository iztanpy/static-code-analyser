#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "qps/query_evaluator/constraint.h"

namespace ResultFormatter {
void CombinationWithTable(
    const std::unordered_map<ColName, std::vector<Cell>>& umap,
    const Table& table, const std::vector<ColName>& order);
}  // namespace ResultFormatter
