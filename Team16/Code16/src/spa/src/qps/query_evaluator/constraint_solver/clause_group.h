#pragma once

#include <queue>
#include <unordered_map>
#include <vector>
#include <cassert>
#include <unordered_set>
#include <iostream>
#include <memory>
#include <string>
#include <climits>
#include <algorithm>
#include <utility>
#include <limits>

#include "qps/clauses/clause.h"
#include "qps/query_evaluator/constraint_solver/constraint_table.h"
#include "PKB/API/ReadFacade.h"

class ClauseGroup {
 public:
  /*!
   * Constructs a ClauseGroup from a ClauseSet
   * @param clause_set
   */
  explicit ClauseGroup(ClauseSet& clause_set);

  /*!
   * Evaluates clauses within the ClauseGroup and return the ConstraintTable of result
   * @param pkb_reader
   * @return ConstraintTable of result
   */
  ConstraintTable Evaluate(ReadFacade& pkb_reader) const;

  /*!
   * Returns the score of the ClauseGroup calculated from the scores of its clauses
   * @return int score
   */
  int Score() const;

  /*!
   * Returns the number of clauses in the ClauseGroup
   * @return int number of clauses
   */
  int SizeForTesting() const;

 private:
  std::vector<std::unique_ptr<Clause>> clauses_;
};
