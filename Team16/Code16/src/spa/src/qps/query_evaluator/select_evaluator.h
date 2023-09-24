#pragma once

#include <string>
#include <unordered_set>
#include <utility>

#include "qps/query_evaluator/constraint_solver/constraint.h"
#include "qps/clauses/select_clause.h"
#include "PKB/API/ReadFacade.h"
#include "utils/hash_utils.h"
#include "utils/entity_types.h"
#include "evaluator_util.h"

/*!
 * A visitor that handles the evaluation of Select clauses
 */
struct SelectEvaluator {
  static UnaryConstraint Evaluate(Declaration& declaration, ReadFacade& pkb_reader);
};
