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
  /*!
   * Adds a clause to the ClauseGrouper
   * @param clause - unary or binary clause, will raise assert error if binary clause
   * is passed in
   */
  void addClause(std::unique_ptr<Clause> clause);

  /*!
   * Returns a vector of ClauseGroup in the order of evaluation according to ClauseGroup::Score()
   * @return vector of ClauseGroup
   */
  std::vector<ClauseGroup> GetClauseGroupOrder();

 private:
  ClauseSet clauses_;
  std::unordered_map<Synonym, Synonym> map_;  // for Union-Find
  Synonym Find(const Synonym& syn);
  void Union(const std::unique_ptr<Clause>& clause);
};
