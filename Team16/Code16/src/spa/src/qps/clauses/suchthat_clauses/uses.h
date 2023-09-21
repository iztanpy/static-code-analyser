#pragma once

#include <utility>
#include <unordered_set>
#include <string>
#include <functional>
#include <optional>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"

class UsesS : public SuchThatClause {
 public:
  UsesS(StmtRef lhs, EntRef rhs);
  StmtRef lhs;
  EntRef rhs;

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};

class UsesP : public SuchThatClause {
 public:
  UsesP(EntRef lhs, EntRef rhs);
  EntRef lhs;
  EntRef rhs;

  Constraint Evaluate(ReadFacade& pkb_reader) override;
 private:
  void Validate() override;
};
