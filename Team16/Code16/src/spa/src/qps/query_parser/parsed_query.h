#pragma once

#include <vector>
#include <memory>
#include <unordered_set>

#include "qps/clauses/select_clause.h"
#include "qps/clauses/suchthat_clauses/suchthat_clause.h"
#include "qps/clauses/pattern_clauses/pattern_clause.h"
#include "utils/clauses_types.h"

/**
 * A struct that represents a parsed query.
 * selects: a vector of synonyms that are selected in the query, vector of size 0 represents select BOOLEAN
 * clauses: a vector of clauses that are in the query
 */
struct ParsedQuery {
  std::vector<synonym> selects;
  std::vector<std::unique_ptr<Clause>> clauses;
};
