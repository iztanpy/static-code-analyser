#pragma once

#include <string>
#include <vector>
#include <memory>
#include "qps/clauses/select_clause.h"
#include "qps/query_parser/clause_builder/select_clause_builder.h"
#include "qps/query_parser/query_tokenizer/query_tokenizer.h"
#include "qps/declaration.h"
#include "qps/query_parser/clause_builder/suchthat_clause_builder.h"
#include "qps/clauses/pattern_clause.h"
#include "qps/query_parser/clause_builder/pattern_clause_builder.h"

class ClauseDirector {
 public:
  static SelectClause makeSelectClause(
      SelectClauseBuilder builder, const QueryToken & token, const std::vector<Declaration> & declarations);
  static std::unique_ptr<SuchThatClause> makeSuchThatClause(
      SuchThatClauseBuilder builder,
      const std::vector<QueryToken> & tokens,
      const std::vector<Declaration> & declarations);
  static std::unique_ptr<PatternClause> makePatternClause(
      PatternClauseBuilder builder,
      const std::vector<QueryToken> & tokens,
      const std::vector<Declaration> & declarations);
};
