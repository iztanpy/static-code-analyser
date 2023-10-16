#pragma once

#include "qps/query_evaluator/constraint_solver/constraint.h"
#include "PKB/API/ReadFacade.h"

/*!
 * Represents a clause in a query
 */
class Clause {
 public:
//  virtual int Score() = 0;
  virtual Constraint Evaluate(ReadFacade& pkb_reader) = 0;
  virtual std::unordered_set<synonym> GetSynonyms() = 0;
  virtual ~Clause() = default;
};
