#pragma once

#include <utility>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"

class Parent : public SuchThatClause {
 public:
  Parent(StmtRef lhs, StmtRef rhs);
  StmtRef lhs;
  StmtRef rhs;

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};
