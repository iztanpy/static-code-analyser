#pragma once

#include <utility>
#include <unordered_set>
#include <string>
#include <cassert>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"
#include "qps/clauses/suchthat_clauses/suchthat_validator.h"

class UsesS : public SuchThatClause {
 public:
  StmtRef lhs;
  EntRef rhs;

  UsesS(StmtRef lhs, EntRef rhs) {
    Validate();
    this->rhs = std::move(rhs);
    this->lhs = std::move(lhs);
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};

class UsesP : public SuchThatClause {
 public:
  EntRef lhs;
  EntRef rhs;

  UsesP(EntRef lhs, EntRef rhs) {
    Validate();
    this->rhs = std::move(rhs);
    this->lhs = std::move(lhs);
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;
 private:
  void Validate() override;
};
