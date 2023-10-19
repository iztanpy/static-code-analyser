#pragma once

#include <utility>
#include "qps/clauses/suchthat_clauses/suchthat_clause.h"
#include "qps/clauses/suchthat_clauses/suchthat_validator.h"
#include "qps/query_evaluator/parent_evaluator.h"

class Next : public SuchThatClause {
 public:
  StmtRef lhs;
  StmtRef rhs;

  RelRefType GetRelRef() const override {
    return RelRefType::NEXT;
  }

  Next(StmtRef lhs, StmtRef rhs)
      : lhs(std::move(lhs)), rhs(std::move(rhs)) {
    Validate();
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};

class NextT : public SuchThatClause {
 public:
  StmtRef lhs;
  StmtRef rhs;

  RelRefType GetRelRef() const override {
    return RelRefType::NEXTT;
  }

  NextT(StmtRef lhs, StmtRef rhs)
      : lhs(std::move(lhs)), rhs(std::move(rhs)) {
    Validate();
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};
