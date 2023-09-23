#pragma once

#include <string>
#include "qps/design_entity.h"
#include "qps/clauses/clause.h"

class SelectClause : public Clause {
 public:
  DesignEntity design_entity;
  std::string synonym;

  /*!
   * Checks if this Select clause is equal to another Select clause
   * @param other select clause
   * @return true if they are equal, else false
   */
  bool equals(const SelectClause & other) const;
};
