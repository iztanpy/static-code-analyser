#pragma once

#include <vector>
#include <memory>

#include "qps/clauses/select_clause.h"
#include "qps/clauses/suchthat_clause.h"

struct ParsedQuery {
  SelectClause select;
  std::vector<std::unique_ptr<SuchThatClause>> such_that_clauses;
  /* TODO: pattern clauses */
};
