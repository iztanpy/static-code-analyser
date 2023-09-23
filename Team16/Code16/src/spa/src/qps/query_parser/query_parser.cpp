#include <memory>
#include <utility>
#include <string>
#include "qps/query_parser/query_parser.h"
#include "qps/query_parser/clause_builder/clause_director.h"
#include "qps/query_parser/clause_builder/suchthat_clause_builder.h"
#include "qps/query_parser/clause_builder/pattern_clause_builder.h"

ParsedQuery QueryParser::ParseTokenizedQuery(std::string & query) {
  ParsedQuery parsedQuery;

  TokenisedQuery tokenised_query = QueryTokenizer::tokenize(query);
  std::vector<Declaration> declarations = tokenised_query.declarations;

  std::vector<QueryToken> selectTokens = tokenised_query.select_tokens;
  std::vector<SelectClause> selectClauses = ExtractSelectClauses(selectTokens, declarations);
  /* For now, we only have one synonym for select clause
   * hence we can just return the first value of the vector
   * - open to extensions in the future
   */
  if (selectClauses.size() == 1) {
    parsedQuery.select = selectClauses[0];
  }

  std::vector<QueryToken> suchThatTokens = tokenised_query.such_that_tokens;
  if (!suchThatTokens.empty()) {
    std::vector<std::unique_ptr<SuchThatClause>> suchThatClauses = ExtractSuchThatClauses(suchThatTokens, declarations);
    parsedQuery.such_that_clauses = std::move(suchThatClauses);
  }

  std::vector<QueryToken> patternTokens = tokenised_query.pattern_tokens;
  if (!patternTokens.empty()) {
    std::vector<std::unique_ptr<PatternClause>> patternClauses = ExtractPatternClauses(patternTokens, declarations);
    parsedQuery.pattern_clauses = std::move(patternClauses);
  }
  return parsedQuery;
}

std::vector<SelectClause> QueryParser::ExtractSelectClauses(const std::vector<QueryToken> & selectTokens,
                                                            const std::vector<Declaration> & declarations) {
  std::vector<SelectClause> selectClauses;
  // invoke builder design pattern
  for (const QueryToken & token : selectTokens) {
    SelectClauseBuilder builder;
    selectClauses.push_back(ClauseDirector::makeSelectClause(builder, token, declarations));
  }
  return selectClauses;
}

std::vector<std::unique_ptr<SuchThatClause>>
QueryParser::ExtractSuchThatClauses(const std::vector<QueryToken> & suchThatTokens,
                                    const std::vector<Declaration> & declarations) {
  std::vector<std::unique_ptr<SuchThatClause>> suchThatClauses;
  // invoke builder design pattern
  for (size_t i = 0; i < suchThatTokens.size(); i += 3) {
    // Such that tokens should be parsed in 3s
    std::vector<QueryToken> singleClause = {suchThatTokens[i], suchThatTokens[i + 1], suchThatTokens[i + 2]};
    SuchThatClauseBuilder builder;
    std::unique_ptr<SuchThatClause> clause = ClauseDirector::makeSuchThatClause(builder, singleClause, declarations);
    suchThatClauses.push_back(std::move(clause));
  }
  return suchThatClauses;
}

std::vector<std::unique_ptr<PatternClause>>
QueryParser::ExtractPatternClauses(const std::vector<QueryToken> & patternTokens,
                                   const std::vector<Declaration> & declarations) {
  std::vector<std::unique_ptr<PatternClause>> patternClauses;
  // invoke builder design pattern
  for (size_t i = 0; i < patternTokens.size(); i += 3) {
    // Such that tokens should be parsed in 3s
    std::vector<QueryToken> singleClause = {patternTokens[i], patternTokens[i + 1], patternTokens[i + 2]};
    PatternClauseBuilder builder;
    std::unique_ptr<PatternClause> clause = ClauseDirector::makePatternClause(builder, singleClause, declarations);
    patternClauses.push_back(std::move(clause));
  }
  return patternClauses;
}

