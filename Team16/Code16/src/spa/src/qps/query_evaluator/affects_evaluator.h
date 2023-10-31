#pragma once

#include <utility>
#include <unordered_set>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"
#include "qps/query_evaluator/evaluator_util.h"
#include "utils/entity_types.h"

struct AffectsEvaluator {
  static bool Handle(int lhs, int rhs, ReadFacade& pkb_reader);

  static bool Handle(int lhs, Wildcard& rhs, ReadFacade& pkb_reader);

  static bool Handle(Wildcard& lhs, int rhs, ReadFacade& pkb_reader);

  static bool Handle(Wildcard& lhs, Wildcard& rhs, ReadFacade& pkb_reader);

  static UnaryConstraint Handle(Declaration& lhs, Wildcard& rhs, ReadFacade& pkb_reader);

  static UnaryConstraint Handle(Wildcard& lhs, Declaration& rhs, ReadFacade& pkb_reader);

  static UnaryConstraint Handle(Declaration& lhs, int rhs, ReadFacade& pkb_reader);

  static UnaryConstraint Handle(int lhs, Declaration& rhs, ReadFacade& pkb_reader);

  static Constraint Handle(Declaration& lhs, Declaration& rhs, ReadFacade& pkb_reader);
};
