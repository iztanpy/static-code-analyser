#pragma once

#include <string>
#include <list>

#include "qps/parsed_query.h"

class QueryEvaluator {
 public:
  std::list<std::string> evaluate(ParsedQuery& query);
};
