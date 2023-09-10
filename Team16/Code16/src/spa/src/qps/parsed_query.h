#pragma once

#include <vector>

#include "qps/select_clause.h"
#include "qps/suchthat_clause.h"

struct ParsedQuery {
  SelectClause select;
  /* TODO: such that and pattern clauses */
  std::vector<SuchThatClause> suchthat_clauses;
};
