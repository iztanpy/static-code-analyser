#pragma once

#include <utility>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"
#include "qps/clauses/suchthat_clauses/suchthat_validator.h"
#include "qps/query_evaluator/parent_evaluator.h"

/*!
 * Represents a Parent clause in a query according to PQL grammar
 */
class Parent : public SuchThatClause {
 public:
  StmtRef lhs;
  StmtRef rhs;

  RelRefType GetRelRef() const override {
    return RelRefType::PARENT;
  }

  Parent(StmtRef lhs, StmtRef rhs, bool is_not)
      : SuchThatClause(lhs, rhs, is_not), lhs(std::move(lhs)), rhs(std::move(rhs)) {
    Validate();
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};

/*!
 * Represents a ParentT clause in a query according to PQL grammar
 */
class ParentT : public SuchThatClause {
 public:
  StmtRef lhs;
  StmtRef rhs;

  RelRefType GetRelRef() const override {
    return RelRefType::PARENTT;
  }

  ParentT(StmtRef lhs, StmtRef rhs, bool is_not)
      : SuchThatClause(lhs, rhs, is_not), lhs(std::move(lhs)), rhs(std::move(rhs)) {
    Validate();
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};
