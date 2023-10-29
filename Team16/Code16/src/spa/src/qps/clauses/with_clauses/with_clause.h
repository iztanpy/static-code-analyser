#pragma once

#include <string>
#include <utility>
#include <unordered_set>
#include "qps/clauses/clause.h"
#include "qps/clauses/attr_ref.h"
#include "qps/clauses/with_clauses/with_evaluator.h"

class WithClause : public Clause {
 public:
  Ref lhs;
  Ref rhs;

  WithClause(Ref lhs, Ref rhs) : lhs(std::move(lhs)), rhs(std::move(rhs)) {}

  /*!
   * Evaluate this clause given info from PKB
   * @param pkb_reader
   * @return Constraint that contains all possible valid values for this clause
   */
  Constraint Evaluate(ReadFacade& pkb_reader) override;

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
  friend bool operator==(const WithClause& lhs, const WithClause& rhs);

  ~WithClause() override = default;

 private:
  /*!
   * Validates the pattern clause semantically
   * Will throw QpsSemanticError if the clause is initialized with invalid arguments
   */
  void Validate();

  /*!
  * Gets the RelRefType of this clause
  * @return
  */
  RelRefType GetRelRef() const override {
    return RelRefType::WITH;
  };

  static RefUnderlyingType GetType(const Ref& param);
};
