#pragma once

#include <string>
#include "qps/clauses/clause.h"
#include "qps/clauses/suchthat_clause.h"

using ExprSpec = std::variant<std::string, Wildcard>;
class PatternClause : public Clause {
 public:
  Declaration syn_assignment;
  EntRef lhs;
  ExprSpec rhs;

  static bool are_expr_spec_equal(ExprSpec expr_1, ExprSpec expr_2);
};

class WildCardPattern : public PatternClause {
 public:
  WildCardPattern(Declaration syn_assignment, EntRef lhs, Wildcard rhs);
  Declaration syn_assignment;
  EntRef lhs;
  Wildcard rhs;
};

class ExactPattern : public PatternClause {
 public:
  ExactPattern(Declaration syn_assignment, EntRef lhs, std::string rhs);
  Declaration syn_assignment;
  EntRef lhs;
  std::string rhs;
};

class PartialPattern : public PatternClause {
 public:
  PartialPattern(Declaration syn_assignment, EntRef lhs, std::string rhs);
  Declaration syn_assignment;
  EntRef lhs;
  std::string rhs;
};
