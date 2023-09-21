#pragma once

#include <utility>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"
#include "qps/clauses/suchthat_clauses/suchthat_validator.h"

class ModifiesP : public SuchThatClause {
 public:
  EntRef lhs;
  EntRef rhs;

  ModifiesP(EntRef lhs, EntRef rhs) {
    Validate();
    this->rhs = std::move(rhs);
    this->lhs = std::move(lhs);
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};

class ModifiesS : public SuchThatClause {
 public:
  StmtRef lhs;
  EntRef rhs;

  ModifiesS(StmtRef lhs, EntRef rhs) {
    Validate();
    this->rhs = std::move(rhs);
    this->lhs = std::move(lhs);
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};
