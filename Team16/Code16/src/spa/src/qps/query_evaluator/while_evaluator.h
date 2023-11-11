#pragma once

#include <string>
#include <unordered_set>

#include "qps/query_evaluator/constraint_solver/constraint.h"
#include "qps/declaration.h"
#include "PKB/API/read_facade.h"
#include "qps/query_evaluator/evaluator_util.h"
#include "utils/clauses_types.h"
#include "utils/entity_types.h"

/*!
 * A visitor that handles the evaluation of while pattern clauses
 * It contains handling function for all combinations of assign pattern clauses parameters
 */
struct WhileEvaluator {
  static BinaryConstraint Handle(std::string& synonym,
                                 Declaration& lhs,
                                 ReadFacade& pkb_reader);

  static UnaryConstraint Handle(std::string& synonym,
                                Wildcard& lhs,
                                ReadFacade& pkb_reader);

  static UnaryConstraint Handle(std::string& synonym,
                                std::string& lhs,
                                ReadFacade& pkb_reader);
};
