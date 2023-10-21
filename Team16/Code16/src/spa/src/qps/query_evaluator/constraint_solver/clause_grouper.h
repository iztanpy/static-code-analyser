#pragma once

#include <unordered_map>
#include <vector>
#include <utility>
#include <algorithm>
#include <memory>

#include "qps/clauses/clause.h"
#include "qps/query_evaluator/constraint_solver/clause_group.h"

class ClauseGrouper {
 public:
  void addClause(std::unique_ptr<Clause> clause);
  std::vector<ClauseGroup> GetClauseGroupOrder();

 private:
  ClauseSet clauses_;
  std::unordered_map<Synonym, Synonym> map_;  // for Union-Find
  Synonym Find(const Synonym& syn);
  void Union(const std::unique_ptr<Clause>& clause);
};
