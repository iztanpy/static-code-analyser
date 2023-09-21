#pragma once

#include <string>
#include <vector>
#include <utility>
#include <unordered_set>

#include "qps/query_parser/parsed_query.h"
#include "qps/query_evaluator/select_evaluator.h"
#include "qps/query_evaluator/constraint_table.h"
#include "PKB/API/ReadFacade.h"

// Entry point for Query Evaluator
class QueryEvaluator {
 private:
  ReadFacade& pkb;

 public:
  explicit QueryEvaluator(ReadFacade& pkb);
  std::unordered_set<std::string> Evaluate(const ParsedQuery& query);
};
