#pragma once

#include <vector>
#include <memory>

#include "qps/clauses/select_clause.h"
#include "qps/clauses/suchthat_clauses/suchthat_clause.h"
#include "qps/clauses/pattern_clause.h"

struct ParsedQuery {
  SelectClause select;
  std::vector<std::unique_ptr<SuchThatClause>> such_that_clauses;
  std::vector<std::unique_ptr<PatternClause>> pattern_clauses;
};
