#pragma once

#include <string>
#include <vector>
#include <utility>

#include "qps/query_parser/parsed_query.h"
#include "qps/query_evaluator/select_evaluator.h"
#include "PKB/API/ReadFacade.h"

// Entry point for Query Evaluator
class QueryEvaluator {
 private:
  ReadFacade & pkb;

 public:
  explicit QueryEvaluator(ReadFacade & pkb);
  std::vector<std::string> Evaluate(const ParsedQuery & query);
};
