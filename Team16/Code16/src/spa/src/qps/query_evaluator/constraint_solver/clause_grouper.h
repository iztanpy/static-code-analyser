#pragma once

#include <unordered_map>
#include <vector>
#include <utility>
#include <algorithm>
#include <memory>
#include <list>
#include <unordered_set>

#include "qps/clauses/clause.h"
#include "qps/query_evaluator/constraint_solver/clause_group.h"
#include "qps/clauses/attr_ref.h"
#include "qps/clauses/select_clause.h"

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
  std::list<std::unique_ptr<Clause>> clauses_;
  std::unordered_map<Synonym, Synonym> map_;  // for Union-Find

  /*!
   * Finds the root of the synonym in the Union-Find data structure
   * @param syn - synonym to find
   * @return root of the synonym
   */
  Synonym Find(const Synonym& syn);

  /*!
   * Unions the two synonyms in the Union-Find data structure
   * @param clause - clause to be unioned
   */
  void Union(const std::unique_ptr<Clause>& clause);

  /*!
   * This function will look at synonyms_ and not_synonyms_ and
   * top up clauses_ with Select clauses of synonyms that's in not_synonyms but
   * not in synonyms_
   */
  void TopUpSelectClause();
};
