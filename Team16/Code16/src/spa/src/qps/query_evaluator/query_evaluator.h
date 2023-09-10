#pragma once

#include <string>
#include <vector>
#include <utility>

#include "qps/parsed_query.h"
#include "qps/query_evaluator/select_evaluator.h"
#include "pkb/api/read_facade.h"

// Entry point for Query Evaluator
class QueryEvaluator {
 private:
  ReadFacade& pkb;

 public:
  explicit QueryEvaluator(ReadFacade& pkb);
  std::vector<std::string> Evaluate(const ParsedQuery& query);
};
