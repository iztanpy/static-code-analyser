#pragma once

#include <utility>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"
#include "qps/clauses/suchthat_clauses/suchthat_validator.h"
#include "qps/query_evaluator/parent_evaluator.h"

class Calls : public SuchThatClause {
 public:
  EntRef lhs;
  EntRef rhs;

  Calls(EntRef lhs, EntRef rhs)
      : lhs(std::move(lhs)), rhs(std::move(rhs)) {
    Validate();
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};

class CallsT : public SuchThatClause {
 public:
  EntRef lhs;
  EntRef rhs;

  CallsT(EntRef lhs, EntRef rhs)
      : lhs(std::move(lhs)), rhs(std::move(rhs)) {
    Validate();
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};
