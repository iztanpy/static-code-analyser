#pragma once

#include <string>
#include <variant>
#include <unordered_set>

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
  Constraint Evaluate(ReadFacade& pkb_reader) override = 0;

  /*!
   * Gets the synonyms used in this Pattern clause.
   * @return a set of elements
   */
  std::unordered_set<synonym> GetSynonyms() override;

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
};

class AssignPattern : public PatternClause {
 public:
  Declaration syn_assignment;
  EntRef lhs;
  ExprSpec rhs;

  AssignPattern(Declaration syn_assignment, EntRef lhs, ExprSpec rhs)
      : syn_assignment(syn_assignment), lhs(lhs), rhs(rhs) {
    Validate();
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};

class WhilePattern : public PatternClause {
 public:
  Declaration syn_assignment;
  EntRef lhs;

  WhilePattern(Declaration syn_assignment, EntRef lhs) : syn_assignment(syn_assignment), lhs(lhs) {
    Validate();
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};

class IfPattern : public PatternClause {
 public:
  Declaration syn_assignment;
  EntRef lhs;

  IfPattern(Declaration syn_assignment, EntRef lhs) : syn_assignment(syn_assignment), lhs(lhs) {
    Validate();
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};
