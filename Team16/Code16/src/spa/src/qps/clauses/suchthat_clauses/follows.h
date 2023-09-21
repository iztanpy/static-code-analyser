#pragma once

#include <utility>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"
#include "qps/clauses/suchthat_clauses/suchthat_validator.h"

class Follows : public SuchThatClause {
 public:
  StmtRef lhs;
  StmtRef rhs;

  Follows(StmtRef lhs, StmtRef rhs) {
    Validate();
    this->rhs = std::move(rhs);
    this->lhs = std::move(lhs);
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};

class FollowsT : public SuchThatClause {
 public:
  StmtRef lhs;
  StmtRef rhs;

  FollowsT(StmtRef lhs, StmtRef rhs) {
    Validate();
    this->rhs = std::move(rhs);
    this->lhs = std::move(lhs);
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};
