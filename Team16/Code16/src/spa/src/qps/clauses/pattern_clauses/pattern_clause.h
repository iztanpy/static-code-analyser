#pragma once

#include <string>
#include <variant>

#include "qps/query_evaluator/constraint_solver/constraint.h"
#include "qps/clauses/clause.h"
#include "qps/clauses/pattern_clauses/pattern_validator.h"
#include "qps/query_evaluator/assign_pattern_evaluator.h"
#include "PKB/API/ReadFacade.h"
#include "utils/clauses_types.h"
#include "utils/entity_types.h"

/*!
 * Represents a pattern clause in a query
 */
class PatternClause : public Clause {
 public:
  /*!
   * Checks if two expression-specs are equal
   * @param expr_1 first expression-spec
   * @param expr_2 second expression-spec
   * @return true if they are equal, else false
   */
  static bool are_expr_spec_equal(ExprSpec expr_1, ExprSpec expr_2);

  /*!
   * Evaluate this clause given info from PKB
   * @param pkb_reader
   * @return Constraint that contains all possible valid values for this clause
   */
  virtual Constraint Evaluate(ReadFacade& pkb_reader) = 0;
  ~PatternClause() override = default;

 private:
  /*!
   * Validates the pattern clause semantically
   * Will throw QpsSemanticError if the clause is initialized with invalid arguments
   */
  virtual void Validate() = 0;

  /*!
   * Pattern clause are in the format syn_assignment(lhs, rhs)
   */
  Declaration syn_assignment;
  EntRef lhs;
  ExprSpec rhs;
};

/*!
 * Represents a pattern clause in the format syn_assignment(lhs, rhs) where rhs is a wildcard
 */
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

/*!
 * Represents a pattern clause in the format syn_assignment(lhs, rhs) where rhs is an exact pattern
 */
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

/*!
 * Represents a pattern clause in the format syn_assignment(lhs, rhs) where rhs is an partial pattern
 */
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
