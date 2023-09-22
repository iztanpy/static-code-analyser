#pragma once

#include <string>
#include <unordered_set>
#include <utility>

#include "qps/query_evaluator/constraint.h"
#include "qps/clauses/select_clause.h"
#include "PKB/API/ReadFacade.h"
#include "utils/hash_utils.h"
#include "utils/StmtEntity.h"
#include "qps/clauses/evaluator_util.h"

// Evaluates the Select clause of a query
class SelectEvaluator {
 private:
  ReadFacade& pkb_reader;
  const SelectClause select_clause;

 public:
  SelectEvaluator(ReadFacade& pkb_reader, const SelectClause select_clause);
  UnaryConstraint Evaluate();
};
