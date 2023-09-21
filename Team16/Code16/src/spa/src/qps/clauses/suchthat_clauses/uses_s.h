#pragma once

#include <utility>
#include <unordered_set>
#include <string>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"

class UsesS : public SuchThatClause {
 public:
  UsesS(StmtRef lhs, EntRef rhs);
  StmtRef lhs;
  EntRef rhs;

  Constraint Evaluate(ReadFacade& pkb_reader) override;
};
