#pragma once

#include <string>
#include <unordered_set>
#include <cassert>
#include <utility>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"
#include "qps/query_evaluator/evaluator_util.h"
#include "utils/entity_types.h"

/*!
 * A visitor that handles the evaluation of Follows/FollowsT clauses
 * It contains handling function for all combinations of Follows/FollowsT clauses parameters
 */
struct FollowsEvaluator {
  static bool Handle(int lhs, int rhs, ReadFacade& pkb_reader, bool is_FollowT);

  static UnaryConstraint Handle(int lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_FollowT);

  static bool Handle(int lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_FollowT);

  static UnaryConstraint Handle(Declaration& lhs, int rhs, ReadFacade& pkb_reader, bool is_FollowT);

  static Constraint Handle(Declaration& lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_FollowT);

  static UnaryConstraint Handle(Declaration& lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_FollowT);

  static bool Handle(Wildcard& lhs, int rhs, ReadFacade& pkb_reader, bool is_FollowT);

  static UnaryConstraint Handle(Wildcard& lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_FollowT);

  static bool Handle(Wildcard& lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_FollowT);
};
