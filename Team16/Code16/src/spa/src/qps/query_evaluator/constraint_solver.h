#pragma once

#include <string>
#include <vector>
#include <unordered_set>

#include "qps/query_evaluator/constraint.h"
#include "qps/clauses/select_clause.h"

// WIP: This will accept a list of constraint, it will solve them and allow
// us to select a subset of synonyms. The logic of solving the constraints
// will be housed within here
class ConstraintSolver {
 public:
  std::vector<std::string> Solve(const SelectClause & select_clause, const std::vector<Constraint> & constraints);
};
