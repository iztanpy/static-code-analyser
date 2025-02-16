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
 * A visitor that handles the evaluation of assign pattern clauses
 * It contains handling function for all combinations of assign pattern clauses parameters
 */
struct AssignPatternEvaluator {
  static BinaryConstraint Handle(std::string& assign_synonym,
                                 Declaration& lhs,
                                 ExactExpr& rhs,
                                 ReadFacade& pkb_reader);

  static BinaryConstraint Handle(std::string& assign_synonym,
                                 Declaration& lhs,
                                 PartialExpr& rhs,
                                 ReadFacade& pkb_reader);

  static BinaryConstraint Handle(std::string& assign_synonym, Declaration& lhs, Wildcard& rhs, ReadFacade& pkb_reader);

  static UnaryConstraint Handle(std::string& assign_synonym,
                                Wildcard& lhs,
                                ExactExpr& rhs,
                                ReadFacade& pkb_reader);

  static UnaryConstraint Handle(std::string& assign_synonym,
                                Wildcard& lhs,
                                PartialExpr& rhs,
                                ReadFacade& pkb_reader);

  static UnaryConstraint Handle(std::string& assign_synonym, Wildcard& lhs, Wildcard& rhs, ReadFacade& pkb_reader);

  static UnaryConstraint Handle(std::string& assign_synonym,
                                std::string& lhs,
                                ExactExpr& rhs,
                                ReadFacade& pkb_reader);

  static UnaryConstraint Handle(std::string& assign_synonym,
                                std::string& lhs,
                                PartialExpr& rhs,
                                ReadFacade& pkb_reader);

  static UnaryConstraint Handle(std::string& assign_synonym, std::string& lhs, Wildcard& rhs, ReadFacade& pkb_reader);
};
