#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include "PKB/API/ReadFacade.h"
#include "qps/query_tokenizer.h"
#include "qps/query_parser.h"
#include "qps/query_evaluator/query_evaluator.h"

class QPS {
 public:
  explicit QPS(ReadFacade &pkb_reader);

  std::unordered_set<std::string> Evaluate(std::string query);

 private:
  ReadFacade &pkb_reader;
};
