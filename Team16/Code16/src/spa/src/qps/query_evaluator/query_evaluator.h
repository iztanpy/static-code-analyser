#pragma once

#include <string>
#include <vector>
#include <utility>
#include <unordered_set>
#include <memory>

#include "qps/query_parser/parsed_query.h"
#include "qps/query_evaluator/constraint_solver/constraint_table.h"
#include "PKB/API/ReadFacade.h"
#include "qps/query_evaluator/constraint_solver/clause_grouper.h"

/*!
 * A class that handles the evaluation of a query
 */
class QueryEvaluator {
 private:
  ReadFacade& pkb;

 public:
  explicit QueryEvaluator(ReadFacade& pkb);

  /*!
   * Evaluates a query
   * @param query is the parsed query
   * @return a set of strings that contains the values of the selected synonyms
   */
  std::unordered_set<std::string> Evaluate(ParsedQuery& query);
};
