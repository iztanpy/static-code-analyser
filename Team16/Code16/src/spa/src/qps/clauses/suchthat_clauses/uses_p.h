#pragma once

#include <utility>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"

class UsesP : public SuchThatClause {
 public:
  UsesP(EntRef lhs, EntRef rhs);
  EntRef lhs;
  EntRef rhs;

  Constraint Evaluate(ReadFacade& pkb_reader) override;
};
