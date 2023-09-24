#pragma once

#include <string>
#include <variant>

#include "qps/query_evaluator/constraint_solver/constraint.h"
#include "qps/clauses/clause.h"
#include "pattern_validator.h"
#include "qps/query_evaluator/assign_pattern_evaluator.h"
#include "PKB/API/ReadFacade.h"
#include "utils/clauses_types.h"

class PatternClause : public Clause {
 public:
  Declaration syn_assignment;
  EntRef lhs;
  ExprSpec rhs;

  /*!
   * Checks if two expression-specs are equal
   * @param expr_1 first expression-spec
   * @param expr_2 second expression-spec
   * @return true if they are equal, else false
   */
  static bool are_expr_spec_equal(ExprSpec expr_1, ExprSpec expr_2);

  // TODO(phuccuongngo99): Can we put this virtual method within
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
