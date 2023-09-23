#pragma once

#include <vector>
#include <memory>
#include <string>
#include "qps/query_parser/parsed_query.h"
#include "qps/query_parser/query_tokenizer/query_tokenizer.h"
#include "qps/clauses/select_clause.h"
#include "qps/declaration.h"
#include "qps/clauses/pattern_clause.h"

class QueryParser {
 public:
  /*!
   * Parses a query
   * @param query is the query string to be parsed
   * @return a ParsedQuery object
   */
  static ParsedQuery ParseTokenizedQuery(std::string & query);

  /*!
   * Creates select clauses given tokens and declarations
   * @param selectTokens are tokens specific to select clause
   * @param declarations the set of declarations
   * @return a vector of select clauses
   */
  static std::vector<SelectClause>
  ExtractSelectClauses(const std::vector<QueryToken> & selectTokens, const std::vector<Declaration> & declarations);

  /*!
   * Create such that clauses given tokens and declarations
   * @param suchThatTokens are the set of tokens specific to such that clause
   * @param declarations the set of declarations
   * @return a vector of pointers to such that clauses
   */
  static std::vector<std::unique_ptr<SuchThatClause>>
  ExtractSuchThatClauses(const std::vector<QueryToken> & suchThatTokens, const std::vector<Declaration> & declarations);

  /*!
   * Creates pattern clauses given tokens and declarations
   * @param patternTokens are the set of tokens specific to pattern clause
   * @param declarations the set of declarations
   * @return a vector of pointers to pattern clauses
   */
  static std::vector<std::unique_ptr<PatternClause>>
  ExtractPatternClauses(const std::vector<QueryToken> & patternTokens, const std::vector<Declaration> & declarations);
};
