#pragma once

#include <string>
#include <vector>
#include <unordered_set>

#include "qps/query_evaluator/constraint.h"
#include "qps/select_clause.h"

class ConstraintSolver {
 public:
  std::vector<std::string> Solve(const SelectClause& select_clause, const std::vector<Constraint>& constraints);
};
