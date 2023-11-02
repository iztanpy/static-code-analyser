#pragma once

#include <string>
#include <utility>
#include <variant>
#include <unordered_set>

#include "qps/query_evaluator/constraint_solver/constraint.h"
#include "qps/clauses/clause.h"
#include "qps/clauses/pattern_clauses/pattern_validator.h"
#include "qps/query_evaluator/assign_pattern_evaluator.h"
#include "qps/query_evaluator/if_evaluator.h"
#include "qps/query_evaluator/while_evaluator.h"
#include "PKB/API/ReadFacade.h"
#include "utils/clauses_types.h"
#include "utils/entity_types.h"

/*!
 * Represents a pattern clause in a query
 */
class PatternClause : public Clause {
 public:
  /*!
   * Pattern clause are in the format syn_assignment(lhs, rhs)
   */
  Declaration declaration;
  EntRef lhs;

  PatternClause(Declaration declaration, EntRef lhs, bool is_not) :
      Clause(is_not), declaration(std::move(declaration)), lhs(lhs) {
  }

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
  std::unordered_set<Synonym> GetSynonyms() const override;

  /*!
   * Functions to support hashing of clauses
   */
  size_t Hash() const override;

  bool equals(const Clause* other) const override;

  // Overloaded == operator
  friend bool operator==(const PatternClause& lhs, const PatternClause& rhs);

  ~PatternClause() override = default;

 private:
  /*!
   * Validates the pattern clause semantically
   * Will throw QpsSemanticError if the clause is initialized with invalid arguments
   */
  virtual void Validate() = 0;
  RelRefType GetRelRef() const override = 0;
};

class AssignPattern : public PatternClause {
 public:
  ExprSpec rhs;

  RelRefType GetRelRef() const override {
    return RelRefType::ASSIGN;
  }

  AssignPattern(Declaration syn, EntRef lhs, ExprSpec rhs, bool is_not)
      : PatternClause(syn, lhs, is_not), rhs(rhs) {
    Validate();
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

  bool equals(const Clause* other) const override;

  // Overloaded == operator
  friend bool operator==(const AssignPattern& lhs, const AssignPattern& rhs);

  /*!
   * Functions to support hashing of assign pattern
   * because it has an extra field ExprSpec
   */
  size_t Hash() const override;

 private:
  void Validate() override;
};

class WhilePattern : public PatternClause {
 public:
  WhilePattern(Declaration syn, EntRef lhs, bool is_not) : PatternClause(syn, lhs, is_not) {
    Validate();
  }

  RelRefType GetRelRef() const override {
    return RelRefType::WHILE;
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};

class IfPattern : public PatternClause {
 public:
  IfPattern(Declaration syn, EntRef lhs, bool is_not) : PatternClause(syn, lhs, is_not) {
    Validate();
  }

  RelRefType GetRelRef() const override {
    return RelRefType::IF;
  }

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  void Validate() override;
};
