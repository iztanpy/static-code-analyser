#pragma once

#include <utility>
#include <unordered_set>
#include <string>
#include <cassert>

#include "qps/clauses/suchthat_clauses/suchthat_validator.h"
#include "qps/query_evaluator/uses_evaluator.h"

/*!
 * Represents a UsesS clause in a query according to PQL grammar
 */
class UsesS : public SuchThatClause {
 public:
  StmtRef lhs;
  EntRef rhs;

  RelRefType GetRelRef() override {
    return RelRefType::USESS;
  }

  UsesS(StmtRef lhs, EntRef rhs)
      : lhs(std::move(lhs)), rhs(std::move(rhs)) {
    Validate();
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};

/*!
 * Represents a UsesP clause in a query according to PQL grammar
 */
class UsesP : public SuchThatClause {
 public:
  EntRef lhs;
  EntRef rhs;

  RelRefType GetRelRef() override {
    return RelRefType::USESP;
  }

  UsesP(EntRef lhs, EntRef rhs)
      : lhs(std::move(lhs)), rhs(std::move(rhs)) {
    Validate();
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;
 private:
  void Validate() override;
};
