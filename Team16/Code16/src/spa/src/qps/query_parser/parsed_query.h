#pragma once

#include <vector>

#include "qps/clauses/select_clause.h"
#include "qps/clauses/suchthat_clause.h"

struct ParsedQuery {
  SelectClause select;
  std::vector<SuchThatClause> suchthat_clauses;
  /* TODO: pattern clauses */
};
