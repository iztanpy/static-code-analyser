#pragma once

#include <unordered_set>
#include <memory>
#include <optional>

#include "qps/query_evaluator/constraint_solver/constraint.h"
#include "PKB/API/ReadFacade.h"
#include "qps/rel_ref.h"

/*!
 * Represents a clause in a query
 */
class Clause {
 private:
  bool is_not_;
  mutable std::optional<std::unordered_set<Synonym>> symnoyms_cache_;

 public:
  explicit Clause(bool is_not) : is_not_(is_not) {}

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
  virtual std::unordered_set<Synonym> ComputeSynonyms() const = 0;

  /*!
   * Gets the synonyms involved in this clause. Use the cached if computed
   * @return the synonyms involved in this clause
   */
  std::unordered_set<Synonym> GetSynonyms() const {
    if (!symnoyms_cache_.has_value()) {
      symnoyms_cache_ = ComputeSynonyms();
    }
    return symnoyms_cache_.value();
  }

  /*!
   * Checks if this clause is equal to another clause.
   * @param other the other clause to compare to
   * @return true if the clauses are equal, false otherwise
   */
  virtual bool equals(const Clause* other) const {
    return GetRelRef() == other->GetRelRef() && is_not_ == other->is_not_;
  }

  /*!
   * Gets the hash of this clause.
   * @return hash of this clause
   */
  virtual size_t Hash() const {
    return std::hash<int>{}(static_cast<int>(GetRelRef())) ^ (std::hash<bool>{}(is_not_) << 1);
  }

  /*!
   * Checks if this clause is a has a not attached to it
   * @return true if it has a not attached, else false
   */
  bool IsNot() const { return is_not_; }

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
