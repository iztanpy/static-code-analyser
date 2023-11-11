#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include "PKB/API/read_facade.h"
#include "qps/query_parser/query_tokenizer/query_tokenizer.h"
#include "qps/query_parser/query_parser.h"
#include "qps/query_evaluator/query_evaluator.h"
#include "qps/qps_errors/qps_semantic_error.h"
#include "qps/qps_errors/qps_syntax_error.h"

class QPS {
 public:
  explicit QPS(ReadFacade& pkb_reader);

  std::unordered_set<std::string> Evaluate(std::string& query);

 private:
  QueryEvaluator query_evaluator;
};
