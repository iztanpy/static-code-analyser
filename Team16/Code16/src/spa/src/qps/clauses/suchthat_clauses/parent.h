#pragma once

#include <utility>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"
#include "qps/clauses/suchthat_clauses/suchthat_validator.h"

class Parent : public SuchThatClause {
 public:
  StmtRef lhs;
  StmtRef rhs;

  Parent(StmtRef lhs, StmtRef rhs)
      : lhs(std::move(lhs)), rhs(std::move(rhs)) {
    Validate();
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};

class ParentT : public SuchThatClause {
 public:
  StmtRef lhs;
  StmtRef rhs;

  ParentT(StmtRef lhs, StmtRef rhs)
      : lhs(std::move(lhs)), rhs(std::move(rhs)) {
    Validate();
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};
