#pragma once

#include <utility>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"
#include "qps/clauses/suchthat_clauses/suchthat_validator.h"

class Parent : public SuchThatClause {
 public:
  StmtRef lhs;
  StmtRef rhs;

  Parent(StmtRef lhs, StmtRef rhs) {
    Validate();
    this->rhs = std::move(rhs);
    this->lhs = std::move(lhs);
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};

class ParentT : public SuchThatClause {
 public:
  StmtRef lhs;
  StmtRef rhs;

  ParentT(StmtRef lhs, StmtRef rhs) {
    Validate();
    this->rhs = std::move(rhs);
    this->lhs = std::move(lhs);
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};
