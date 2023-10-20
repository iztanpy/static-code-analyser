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

#include "qps/clauses/clause.h"
#include "qps/query_evaluator/constraint_solver/constraint_table.h"
#include "PKB/API/ReadFacade.h"

class ClauseGroup {
 public:
  explicit ClauseGroup(ClauseSet& clause_set);

  ConstraintTable Evaluate(ReadFacade& pkb_reader) const;

  int Score() const;

 private:
  std::vector<std::unique_ptr<Clause>> clauses_;
};
