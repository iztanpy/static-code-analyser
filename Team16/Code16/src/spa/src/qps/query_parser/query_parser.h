#pragma once

#include <vector>
#include <memory>
#include "qps/query_parser/parsed_query.h"
#include "qps/query_parser/query_tokenizer/query_tokenizer.h"
#include "qps/clauses/select_clause.h"
#include "qps/declaration.h"

class QueryParser {
 public:
  static ParsedQuery ParseTokenizedQuery(const std::vector<QueryToken> & tokens);
  static std::vector<Declaration> ExtractDeclarations(const std::vector<QueryToken> & tokens);
  static std::vector<SelectClause>
  ExtractSelectClauses(const std::vector<QueryToken> & selectTokens, const std::vector<Declaration> & declarations);
  static std::vector<QueryToken> ExtractSelectTokens(const std::vector<QueryToken> & tokens);
  static std::vector<std::unique_ptr<SuchThatClause>>
  ExtractSuchThatClauses(const std::vector<QueryToken> & suchThatTokens, const std::vector<Declaration> & declarations);
  static std::vector<QueryToken> ExtractSuchThatTokens(const std::vector<QueryToken> & tokens);
};
