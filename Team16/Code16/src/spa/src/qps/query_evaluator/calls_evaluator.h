#pragma once

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"
#include "qps/query_evaluator/evaluator_util.h"
#include "utils/entity_types.h"

/*!
 * A visitor that handles the evaluation of Calls/CallsT clauses
 * It contains handling function for all combinations of Calls/CallsT clauses parameters
 */
struct CallsEvaluator {
  static Constraint Handle(Declaration& lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_CallT);
  static UnaryConstraint Handle(Declaration& lhs, std::string& rhs, ReadFacade& pkb_reader, bool is_CallT);
  static UnaryConstraint Handle(Declaration& lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_CallT);
  static UnaryConstraint Handle(std::string& lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_CallT);
  static bool Handle(std::string& lhs, std::string& rhs, ReadFacade& pkb_reader, bool is_CallT);
  static bool Handle(std::string& lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_CallT);
  static UnaryConstraint Handle(Wildcard& lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_CallT);
  static bool Handle(Wildcard& lhs, std::string& rhs, ReadFacade& pkb_reader, bool is_CallT);
  static bool Handle(Wildcard& lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_CallT);
};
