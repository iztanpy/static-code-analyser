#pragma once

#include <string>
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

  UnaryConstraint Evaluate(ReadFacade& pkb_reader);
};
