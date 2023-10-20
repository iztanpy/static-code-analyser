#pragma once

#include <unordered_set>
#include <memory>

#include "qps/query_evaluator/constraint_solver/constraint.h"
#include "PKB/API/ReadFacade.h"
#include "qps/rel_ref.h"

/*!
 * Represents a clause in a query
 */
class Clause {
 public:
  /*!
   * Gets the relationship reference of this clause.
   * @return the relationship reference of this clause
   */
  virtual RelRefType GetRelRef() const = 0;

  /*!
   * Evaluates this clause.
   * @param pkb_reader the PKB ReadFacade
   * @return the constraint that this clause evaluates to
   */
  virtual Constraint Evaluate(ReadFacade& pkb_reader) = 0;

  /*!
   * Gets the synonyms involved in this clause.
   * @return the synonyms involved in this clause
   */
  virtual std::unordered_set<Synonym> GetSynonyms() const = 0;

  /*!
   * Checks if this clause is equal to another clause.
   * @param other the other clause to compare to
   * @return true if the clauses are equal, false otherwise
   */
  virtual bool equals(const Clause* other) const = 0;

  /*!
   * Gets the hash of this clause.
   * @return hash of this clause
   */
  virtual size_t Hash() const { return std::hash<int>{}(static_cast<int>(GetRelRef())); }

  virtual ~Clause() = default;

  /*!
   * Gets the score of this clause.
   * @return the score of this clause
   */
  int Score() const {
    return RelRef::getClauseScore(GetRelRef(), GetSynonyms().size());
  }
};

struct ClauseHasher {
  size_t operator()(const std::unique_ptr<Clause>& clause) const {
    return clause->Hash();
  }
};

struct ClauseEquality {
  bool operator()(const std::unique_ptr<Clause>& lhs, const std::unique_ptr<Clause>& rhs) const {
    return lhs->equals(rhs.get());
  }
};

using ClauseSet = std::unordered_set<std::unique_ptr<Clause>, ClauseHasher, ClauseEquality>;
