#pragma once

#include "qps/query_evaluator/constraint_solver/constraint.h"
#include "PKB/API/ReadFacade.h"

/*!
 * Represents a clause in a query
 */
class Clause {
 public:
  virtual ~Clause() = default;
};
