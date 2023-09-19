#pragma once

#include "clause.h"
#include "suchthat_clause.h"

using ExprSpec = std::variant<std::string, Wildcard>;
class PatternClause : public Clause {
 public:
  Declaration syn_assignment;
  EntRef lhs;
  ExprSpec rhs;
};

class WildCardPattern : public PatternClause {
 public:
  WildCardPattern(EntRef lhs, Wildcard rhs);
  EntRef lhs;
  Wildcard rhs;
};

class ExactPattern : public PatternClause {
 public:
  ExactPattern(EntRef lhs, std::string rhs);
  EntRef lhs;
  std::string rhs;
};

class PartialPattern : public PatternClause {
 public:
  PartialPattern(EntRef lhs, std::string rhs);
  EntRef lhs;
  std::string rhs;
};