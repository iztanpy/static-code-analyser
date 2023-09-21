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
  static ParsedQuery ParseTokenizedQuery(std::string & query);
  static std::vector<SelectClause>
  ExtractSelectClauses(const std::vector<QueryToken> & selectTokens, const std::vector<Declaration> & declarations);
  static std::vector<std::unique_ptr<SuchThatClause>>
  ExtractSuchThatClauses(const std::vector<QueryToken> & suchThatTokens, const std::vector<Declaration> & declarations);
  static std::vector<std::unique_ptr<PatternClause>>
  ExtractPatternClauses(const std::vector<QueryToken> & patternTokens, const std::vector<Declaration> & declarations);
  static std::vector<QueryToken> ExtractPatternTokens(const std::vector<QueryToken> & tokens);
};
