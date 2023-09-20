#pragma once

#include <utility>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"

class ParentS : public SuchThatClause {
 public:
  ParentS(StmtRef lhs, StmtRef rhs);
  StmtRef lhs;
  StmtRef rhs;

  Constraint Evaluate(ReadFacade& pkb_reader) override;
};
