#pragma once

#include <string>
#include <vector>
#include <memory>
#include "qps/clauses/select_clause.h"
#include "qps/query_parser/clause_builder/select_clause_builder.h"
#include "qps/query_parser/query_tokenizer/query_tokenizer.h"
#include "qps/declaration.h"
#include "qps/query_parser/clause_builder/suchthat_clause_builder.h"
#include "qps/clauses/pattern_clauses/pattern_clause.h"
#include "qps/query_parser/clause_builder/pattern_clause_builder.h"

class ClauseDirector {
 public:
  /*!
   * Builds a select clause
   * @param builder is the select clause builder
   * @param token is a query token
   * @param declarations is the set of declarations
   * @return a Select clause
   */
  static SelectClause makeSelectClause(
      SelectClauseBuilder builder, const QueryToken& token, const std::vector<Declaration>& declarations);

  /*!
   * Builds a Such that clause
   * @param builder is the such that clause builder
   * @param tokens is a vector of query tokens
   * @param declarations is the set of declarations
   * @return a pointer to a such that clause
   */
  static std::unique_ptr<SuchThatClause> makeSuchThatClause(
      SuchThatClauseBuilder builder,
      const std::vector<QueryToken>& tokens,
      const std::vector<Declaration>& declarations);

  /*!
   * Builds a Patter clause
   * @param builder is the pattern clause builder
   * @param tokens is a vector of query tokens
   * @param declarations is the set of declarations
   * @return a pointer to a pattern clause
   */
  static std::unique_ptr<PatternClause> makePatternClause(
      PatternClauseBuilder builder,
      const std::vector<QueryToken>& tokens,
      const std::vector<Declaration>& declarations);
};
