#include <memory>
#include <utility>
#include <string>
#include "qps/query_parser/query_parser.h"
#include "qps/query_parser/clause_builder/clause_director.h"
#include "qps/query_parser/clause_builder/suchthat_clause_builder.h"
#include "qps/query_parser/clause_builder/pattern_clause_builder.h"

ParsedQuery QueryParser::ParseTokenizedQuery(std::string& query) {
  ParsedQuery parsedQuery;

  TokenisedQuery tokenised_query = QueryTokenizer::tokenize(query);
  std::vector<Declaration> declarations = tokenised_query.declarations;

  std::vector<QueryToken> selectTokens = tokenised_query.select_tokens;

  std::vector<std::unique_ptr<Clause>> clauses;
  std::vector<synonym> selects;

  /* For now, we only have one synonym for select clause
   * hence we can just return the first value of the vector
   * - open to extensions in the future
   */
  if (!selectTokens.empty()) {
    std::vector<std::unique_ptr<Clause>> selectClauses = ExtractSelectClauses(selectTokens, declarations);

    for (const std::unique_ptr<Clause>& clause : selectClauses) {
      selects.push_back(*clause->GetSynonyms().begin());
    }

    std::move(selectClauses.begin(), selectClauses.end(), std::back_inserter(clauses));

    selectClauses.clear();
  }

  std::vector<QueryToken> suchThatTokens = tokenised_query.such_that_tokens;
  if (!suchThatTokens.empty()) {
    std::vector<std::unique_ptr<Clause>> suchThatClauses = ExtractSuchThatClauses(suchThatTokens, declarations);
    std::move(suchThatClauses.begin(), suchThatClauses.end(), std::back_inserter(clauses));
    suchThatClauses.clear();
  }

  std::vector<QueryToken> patternTokens = tokenised_query.pattern_tokens;
  if (!patternTokens.empty()) {
    std::vector<std::unique_ptr<Clause>> patternClauses = ExtractPatternClauses(patternTokens, declarations);
    std::move(patternClauses.begin(), patternClauses.end(), std::back_inserter(clauses));
    patternClauses.clear();
  }

  return {selects, std::move(clauses)};
}

std::vector<std::unique_ptr<Clause>> QueryParser::ExtractSelectClauses(const std::vector<QueryToken>& selectTokens,
                                                                       const std::vector<Declaration>& declarations) {
  std::vector<std::unique_ptr<Clause>> selectClauses;
  // invoke builder design pattern
  for (const QueryToken& token : selectTokens) {
    SelectClauseBuilder builder;
    selectClauses.push_back(ClauseDirector::makeSelectClause(builder, token, declarations));
  }
  return selectClauses;
}

std::vector<std::unique_ptr<Clause>>
QueryParser::ExtractSuchThatClauses(const std::vector<QueryToken>& suchThatTokens,
                                    const std::vector<Declaration>& declarations) {
  std::vector<std::unique_ptr<Clause>> suchThatClauses;
  // invoke builder design pattern
  for (size_t i = 0; i < suchThatTokens.size(); i += 3) {
    // Such that tokens should be parsed in 3s
    std::vector<QueryToken> singleClause = {suchThatTokens[i], suchThatTokens[i + 1], suchThatTokens[i + 2]};
    SuchThatClauseBuilder builder;
    std::unique_ptr<Clause> clause = ClauseDirector::makeSuchThatClause(builder, singleClause, declarations);
    suchThatClauses.push_back(std::move(clause));
  }
  return suchThatClauses;
}

std::vector<std::unique_ptr<Clause>>
QueryParser::ExtractPatternClauses(const std::vector<QueryToken>& patternTokens,
                                   const std::vector<Declaration>& declarations) {
  std::vector<std::unique_ptr<Clause>> patternClauses;
  // invoke builder design pattern
  for (size_t i = 0; i < patternTokens.size(); i += 3) {
    // Such that tokens should be parsed in 3s
    std::vector<QueryToken> singleClause = {patternTokens[i], patternTokens[i + 1], patternTokens[i + 2]};
    PatternClauseBuilder builder;
    std::unique_ptr<Clause> clause = ClauseDirector::makePatternClause(builder, singleClause, declarations);
    patternClauses.push_back(std::move(clause));
  }
  return patternClauses;
}

