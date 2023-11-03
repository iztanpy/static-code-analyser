#pragma once

#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "qps/design_entity.h"
#include "qps/clauses/clause.h"
#include "qps/clauses/attr_ref.h"

// TODO(phuccuongngo99): Please change this to point at AttrRef implementation
class SelectClause : public Clause {
 public:
  AttrRef attr_ref;

  explicit SelectClause(AttrRef attr_ref) : Clause(false), attr_ref(std::move(attr_ref)) {}

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
   * only has 1 synonym or 2 synonyms for the Evaluator to group with other clauses
   * @return a set of just 1 element
   */
  std::unordered_set<Declaration> ComputeSynonyms() const override;

  /*!
   * The difference with GetSynonyms() is that this function returns the second synonyms
   * for attrName cases like call.ProcName, read.VarName, and print.VarName
   * while GetSynonyms() returns both call, call.Procname at once as GetSynonyms()
   * reflects the actual number of synonyms produced by Evaluate function
   * @return the synonym to be selected in each select clause
   */
  Declaration GetSelectedSynonym() const;

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
