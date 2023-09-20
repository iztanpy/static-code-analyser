#pragma once

#include <utility>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"

class FollowsT : public SuchThatClause {
 public:
  FollowsT(StmtRef lhs, StmtRef rhs);
  StmtRef lhs;
  StmtRef rhs;

  Constraint Evaluate(ReadFacade& pkb_reader) override;
};
