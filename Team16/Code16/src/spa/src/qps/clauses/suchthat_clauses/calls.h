#pragma once

#include <utility>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"
#include "qps/clauses/suchthat_clauses/suchthat_validator.h"
#include "qps/query_evaluator/calls_evaluator.h"

/*!
 * Represents a Calls clause in a query according to PQL grammar
 */
class Calls : public SuchThatClause {
 public:
  EntRef lhs;
  EntRef rhs;

  RelRefType GetRelRef() const override {
    return RelRefType::CALLS;
  }

  Calls(EntRef lhs, EntRef rhs)
      : lhs(std::move(lhs)), rhs(std::move(rhs)) {
    Validate();
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};

/*!
 * Represents a CallsT clause in a query according to PQL grammar
 */
class CallsT : public SuchThatClause {
 public:
  EntRef lhs;
  EntRef rhs;

  RelRefType GetRelRef() const override {
    return RelRefType::CALLST;
  }

  CallsT(EntRef lhs, EntRef rhs)
      : lhs(std::move(lhs)), rhs(std::move(rhs)) {
    Validate();
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};
