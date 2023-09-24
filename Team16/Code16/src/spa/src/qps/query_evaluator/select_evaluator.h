#pragma once

#include <string>
#include <unordered_set>
#include <utility>

#include "qps/query_evaluator/constraint_solver/constraint.h"
#include "qps/clauses/select_clause.h"
#include "PKB/API/ReadFacade.h"
#include "utils/hash_utils.h"
#include "utils/StmtEntity.h"
#include "evaluator_util.h"

// Evaluates the Select clause of a query
struct SelectEvaluator {
  static UnaryConstraint Evaluate(Declaration& declaration, ReadFacade& pkb_reader);
};
