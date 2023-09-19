#pragma once

#include "qps/query_parser/clause_builder/clause_builder.h"
#include "qps/declaration.h"
#include "qps/clauses/suchthat_clause.h"
#include "qps/clauses/pattern_clause.h"
#include "qps/query_parser/query_tokenizer/query_tokenizer.h"

class PatternClauseBuilder : public ClauseBuilder {
 public:
  Declaration syn_assignment;
  EntRef lhs;
  ExprSpec rhs;
  PQLTokenType rhs_type;  // indicator to know what whether rhs is expr or sub expr

  static void setSynAssignment(Declaration declaration);
  void setLhs(const QueryToken & param, const std::vector<Declaration> & declarations);
  void setRhs(const QueryToken & param, const std::vector<Declaration> & declarations);
  std::unique_ptr<PatternClause> getClause() const;
};
