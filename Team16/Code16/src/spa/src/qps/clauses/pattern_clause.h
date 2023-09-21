#pragma once

#include <string>
#include "qps/clauses/clause.h"
#include "qps/clauses/suchthat_clauses/suchthat_clause.h"
#include "qps/clauses/pattern_validator.h"

using ExprSpec = std::variant<std::string, Wildcard>;
class PatternClause : public Clause {
 public:
  Declaration syn_assignment;
  EntRef lhs;
  ExprSpec rhs;

  static bool are_expr_spec_equal(ExprSpec expr_1, ExprSpec expr_2);

  // TODO(phuccuongngo99): Can we put this virtual method within
  // Clause class instead?
  virtual Constraint Evaluate(ReadFacade& pkb_reader) = 0;
  ~PatternClause() override = default;

 private:
  virtual void Validate() = 0;
};

class WildCardPattern : public PatternClause {
 public:
  Declaration syn_assignment;
  EntRef lhs;
  Wildcard rhs;

  WildCardPattern(Declaration syn_assignment, EntRef lhs, Wildcard rhs)
      : syn_assignment(syn_assignment), lhs(lhs), rhs(rhs) {
    Validate();
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};

class ExactPattern : public PatternClause {
 public:
  Declaration syn_assignment;
  EntRef lhs;
  std::string rhs;

  ExactPattern(Declaration syn_assignment, EntRef lhs, std::string rhs)
      : syn_assignment(syn_assignment), lhs(lhs), rhs(rhs) {
    Validate();
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};

class PartialPattern : public PatternClause {
 public:
  Declaration syn_assignment;
  EntRef lhs;
  std::string rhs;

  PartialPattern(Declaration syn_assignment, EntRef lhs, std::string rhs)
      : syn_assignment(syn_assignment), lhs(lhs), rhs(rhs) {
    Validate();
  }
  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};
