#pragma once

#include <string>
#include <list>

#include "ParsedQuery.h"

class QueryEvaluator {
 public:
  std::list<std::string> evaluate(ParsedQuery& query);
};
