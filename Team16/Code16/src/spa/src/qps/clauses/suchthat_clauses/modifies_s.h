#pragma once

#include <utility>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"

class ModifiesS : public SuchThatClause {
 public:
  ModifiesS(StmtRef lhs, EntRef rhs);
  StmtRef lhs;
  EntRef rhs;

  Constraint Evaluate(ReadFacade& pkb_reader) override;
};
