#pragma once

#include <string>
#include <unordered_set>
#include <utility>

#include "qps/design_entity.h"
#include "qps/clauses/clause.h"
#include "qps/clauses/attr_ref.h"

// TODO(phuccuongngo99): Please change this to point at AttrRef implementation
class SelectClause : public Clause {
 public:
  AttrRef attr_ref;

  explicit SelectClause(AttrRef attr_ref) : attr_ref(std::move(attr_ref)) {}

  /*!
   * Checks if this Select clause is equal to another Select clause
   * @param other select clause
   * @return true if they are equal, else false
   */
  bool equals(const SelectClause& other) const;

  /*!
   * Evaluates this Select clause
   * @param pkb_reader is the PKB ReadFacade
   * @return a UnaryConstraint that contains all possible values of this synonym
   */
  Constraint Evaluate(ReadFacade& pkb_reader) override;

  /*!
   * Gets the synonyms used in this Select clause. However, each select clause
   * only has 1 synonym
   * @return a set of just 1 element
   */
  std::unordered_set<Synonym> GetSynonyms() const override;

  /*!
   * Gets the hash of this Select clause
   * @return the hash of this Select clause
   */
  size_t Hash() const override;

  /*!
   * Checks if this Select clause is equal to another clause
   * @param other clauses of other types
   * @return true if they are equal, else false
   */
  bool equals(const Clause* other) const override;

  /*!
  * Gets the RelRefType of this clause
  * @return
  */
  RelRefType GetRelRef() const override {
    return RelRefType::SELECT;
  }

  // Overloaded == operator
  friend bool operator==(const SelectClause& lhs, const SelectClause& rhs);
};
