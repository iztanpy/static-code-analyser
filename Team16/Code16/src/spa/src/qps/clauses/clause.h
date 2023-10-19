#pragma once

#include <unordered_set>

#include "qps/query_evaluator/constraint_solver/constraint.h"
#include "PKB/API/ReadFacade.h"
#include "qps/rel_ref.h"

/*!
 * Represents a clause in a query
 */
class Clause {
 public:
  virtual RelRefType GetRelRef() = 0;

  virtual Constraint Evaluate(ReadFacade& pkb_reader) = 0;
  virtual std::unordered_set<Synonym> GetSynonyms() = 0;

  // Functions to support hashing of clauses
//  virtual bool equals(const Clause* other) const = 0;
  virtual size_t Hash() const = 0;

  virtual ~Clause() = default;

  int Score() {
    return RelRef::getClauseScore(GetRelRef(), GetSynonyms().size());
  }
};
