#pragma once

#include <string>
#include <unordered_set>

#include "qps/design_entity.h"
#include "qps/clauses/clause.h"
#include "qps/query_evaluator/select_evaluator.h"

class SelectClause : public Clause {
 public:
  Declaration declaration;

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
  std::unordered_set<Synonym> GetSynonyms() override;

  size_t Hash() const override;
};
