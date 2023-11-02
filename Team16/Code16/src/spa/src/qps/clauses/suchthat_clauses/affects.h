#pragma once

#include <utility>
#include "qps/clauses/suchthat_clauses/suchthat_clause.h"
#include "qps/clauses/suchthat_clauses/suchthat_validator.h"
#include "qps/query_evaluator/affects_evaluator.h"

/*!
 * Represents Affects clause in PQL grammar
 */
class Affects : public SuchThatClause {
 public:
  StmtRef lhs;
  StmtRef rhs;

  RelRefType GetRelRef() const override {
    return RelRefType::AFFECTS;
  }

  Affects(StmtRef lhs, StmtRef rhs, bool is_not)
      : SuchThatClause(lhs, rhs, is_not), lhs(std::move(lhs)), rhs(std::move(rhs)) {
    Validate();
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};
