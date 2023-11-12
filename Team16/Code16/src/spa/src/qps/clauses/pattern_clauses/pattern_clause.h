#pragma once

#include <string>
#include <utility>
#include <variant>
#include <unordered_set>

#include "qps/query_evaluator/constraint_solver/constraint.h"
#include "qps/clauses/clause.h"
#include "qps/clauses/pattern_clauses/pattern_validator.h"
#include "PKB/API/read_facade.h"
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
   * Evaluate this clause given info from PKB
   * @param pkb_reader
   * @return Constraint that contains all possible valid values for this clause
   */
  Constraint Evaluate(ReadFacade& pkb_reader) override = 0;

  /*!
   * Gets the synonyms used in this Pattern clause.
   * @return a set of elements
   */
  std::unordered_set<Declaration> ComputeSynonyms() const override;

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
