#pragma once

#include <utility>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"

class ModifiesP : public SuchThatClause {
 public:
  ModifiesP(EntRef lhs, EntRef rhs);
  EntRef lhs;
  EntRef rhs;

  Constraint Evaluate(ReadFacade& pkb_reader) override;
};
