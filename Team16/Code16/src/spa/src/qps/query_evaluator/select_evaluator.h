#pragma once

#include <string>
#include <unordered_set>
#include <utility>

#include "qps/query_evaluator/constraint.h"
#include "qps/select_clause.h"
#include "pkb/api/ReadFacade.h"
#include "utils/hash_utils.h"

// Evaluates the Select clause of a query
class SelectEvaluator {
 private:
  ReadFacade& pkb_reader;
  SelectClause select_clause;

 public:
  SelectEvaluator(ReadFacade& pkb_reader, SelectClause& select_clause);
  Constraint evaluate();
};
