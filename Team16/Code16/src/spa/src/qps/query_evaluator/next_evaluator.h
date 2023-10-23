#pragma once

#include <string>
#include <unordered_set>
#include <cassert>
#include <utility>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"
#include "qps/query_evaluator/evaluator_util.h"
#include "utils/entity_types.h"

/*!
 * A visitor that handles the evaluation of Parent/ParentT clauses
 * It contains handling function for all combinations of Parent/ParentT clauses parameters
 */
struct NextEvaluator {
  static bool Handle(int lhs, int rhs, ReadFacade& pkb_reader, bool is_ParentT);

  static UnaryConstraint Handle(int lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_ParentT);

  static bool Handle(int lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_ParentT);

  static UnaryConstraint Handle(Declaration& lhs, int rhs, ReadFacade& pkb_reader, bool is_ParentT);

  static Constraint Handle(Declaration& lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_ParentT);

  static UnaryConstraint Handle(Declaration& lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_ParentT);

  static bool Handle(Wildcard& lhs, int rhs, ReadFacade& pkb_reader, bool is_ParentT);

  static UnaryConstraint Handle(Wildcard& lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_ParentT);

  static bool Handle(Wildcard& lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_ParentT);
};
