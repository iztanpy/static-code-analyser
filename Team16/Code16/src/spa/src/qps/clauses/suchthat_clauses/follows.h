#pragma once

#include <utility>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"

class Follows : public SuchThatClause {
 public:
  Follows(StmtRef lhs, StmtRef rhs);
  StmtRef lhs;
  StmtRef rhs;

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};
