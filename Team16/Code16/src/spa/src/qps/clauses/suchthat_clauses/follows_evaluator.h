#pragma once

#include <string>
#include <unordered_set>
#include <cassert>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"

struct FollowsEvaluator {
  static bool Handle(int lhs, int rhs, ReadFacade& pkb_reader, bool is_FollowT);

  static UnaryConstraint Handle(int lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_FollowT);

  static bool Handle(int lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_FollowT);

  static UnaryConstraint Handle(Declaration& lhs, int rhs, ReadFacade& pkb_reader, bool is_FollowT);

  static BinaryConstraint Handle(Declaration& lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_FollowT);

  static UnaryConstraint Handle(Declaration& lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_FollowT);

  static bool Handle(Wildcard& lhs, int rhs, ReadFacade& pkb_reader, bool is_FollowT);

  static UnaryConstraint Handle(Wildcard& lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_FollowT);

  static bool Handle(Wildcard& lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_FollowT);
};
