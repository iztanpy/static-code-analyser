#pragma once

#include <utility>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"
#include "qps/clauses/suchthat_clauses/suchthat_validator.h"
#include "qps/query_evaluator/modifies_evaluator.h"

class ModifiesP : public SuchThatClause {
 public:
  EntRef lhs;
  EntRef rhs;

  ModifiesP(EntRef lhs, EntRef rhs)
      : lhs(std::move(lhs)), rhs(std::move(rhs)) {
    Validate();
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};

class ModifiesS : public SuchThatClause {
 public:
  StmtRef lhs;
  EntRef rhs;

  ModifiesS(StmtRef lhs, EntRef rhs)
      : lhs(std::move(lhs)), rhs(std::move(rhs)) {
    Validate();
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};
