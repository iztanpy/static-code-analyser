#pragma once

#include <utility>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"
#include "qps/clauses/suchthat_clauses/suchthat_validator.h"
#include "qps/query_evaluator/follows_evaluator.h"

class Follows : public SuchThatClause {
 public:
  StmtRef lhs;
  StmtRef rhs;

  Follows(StmtRef lhs, StmtRef rhs)
      : lhs(std::move(lhs)), rhs(std::move(rhs)) {
    Validate();
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};

class FollowsT : public SuchThatClause {
 public:
  StmtRef lhs;
  StmtRef rhs;

  FollowsT(StmtRef lhs, StmtRef rhs)
      : lhs(std::move(lhs)), rhs(std::move(rhs)) {
    Validate();
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};
