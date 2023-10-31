#include <memory>
#include <utility>
#include <string>
#include "qps/query_parser/query_parser.h"
#include "qps/query_parser/clause_builder/clause_director.h"
#include "qps/query_parser/clause_builder/suchthat_clause_builder.h"
#include "qps/query_parser/clause_builder/pattern_clause_builder.h"
#include "QueryUtil.h"

ParsedQuery QueryParser::ParseTokenizedQuery(std::string& query) {
  ParsedQuery parsedQuery;

  TokenisedQuery tokenised_query = QueryTokenizer::tokenize(query);
  std::vector<Declaration> declarations = tokenised_query.declarations;

  std::vector<QueryToken> selectTokens = tokenised_query.select_tokens;

  ClauseSet clauses;
  std::vector<Synonym> selects;

  if (!selectTokens.empty()) {
    std::vector<std::unique_ptr<Clause>> selectClauses = ExtractSelectClauses(selectTokens, declarations);

    for (const std::unique_ptr<Clause>& clause : selectClauses) {
      const auto* select_clause = static_cast<const SelectClause*>(clause.get());
      selects.push_back(select_clause->GetSelectedSynonym());
    }

    clauses.insert(std::make_move_iterator(selectClauses.begin()), std::make_move_iterator(selectClauses.end()));
  }

  std::vector<QueryToken> suchThatTokens = tokenised_query.such_that_tokens;
  if (!suchThatTokens.empty()) {
    std::vector<std::unique_ptr<Clause>> suchThatClauses = ExtractSuchThatClauses(suchThatTokens, declarations);
    clauses.insert(std::make_move_iterator(suchThatClauses.begin()), std::make_move_iterator(suchThatClauses.end()));
  }

  std::vector<QueryToken> patternTokens = tokenised_query.pattern_tokens;
  if (!patternTokens.empty()) {
    std::vector<std::unique_ptr<Clause>> patternClauses = ExtractPatternClauses(patternTokens, declarations);
    clauses.insert(std::make_move_iterator(patternClauses.begin()), std::make_move_iterator(patternClauses.end()));
  }

  std::vector<QueryToken> withTokens = tokenised_query.with_tokens;
  if (!withTokens.empty()) {
    std::vector<std::unique_ptr<Clause>> withClauses = ExtractWithClauses(withTokens, declarations);
    clauses.insert(std::make_move_iterator(withClauses.begin()), std::make_move_iterator(withClauses.end()));
  }

  return {selects, std::move(clauses)};
}

std::vector<std::unique_ptr<Clause>> QueryParser::ExtractSelectClauses(const std::vector<QueryToken>& selectTokens,
                                                                       const std::vector<Declaration>& declarations) {
  std::vector<std::unique_ptr<Clause>> selectClauses;
  // If it is select BOOLEAN, return empty select clause
  if (selectTokens.size() == 1) {
    if (QueryUtil::IsSelectBoolean(selectTokens[0].text, declarations)) {
      return selectClauses;
    }
  }
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
  for (size_t i = 0; i < suchThatTokens.size(); i += 4) {
    // Such that tokens should be parsed in 4s
    std::vector<QueryToken> singleClause = {suchThatTokens[i], suchThatTokens[i + 1],
                                            suchThatTokens[i + 2], suchThatTokens[i + 3]};
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
  for (size_t i = 0; i < patternTokens.size(); i += 4) {
    // Pattern tokens should be parsed in 4s
    std::vector<QueryToken> singleClause = {patternTokens[i], patternTokens[i + 1],
                                            patternTokens[i + 2], patternTokens[i + 3]};
    PatternClauseBuilder builder;
    std::unique_ptr<Clause> clause = ClauseDirector::makePatternClause(builder, singleClause, declarations);
    patternClauses.push_back(std::move(clause));
  }
  return patternClauses;
}

std::vector<std::unique_ptr<Clause>> QueryParser::ExtractWithClauses(const std::vector<QueryToken>& withTokens,
                                                                     const std::vector<Declaration>& declarations) {
  std::vector<std::unique_ptr<Clause>> withClauses;
  // invoke builder design pattern
  for (size_t i = 0; i < withTokens.size(); i += 3) {
    // With tokens should be parsed in 3s
    std::vector<QueryToken> singleClause = {withTokens[i], withTokens[i + 1], withTokens[i + 2]};
    WithClauseBuilder builder;
    std::unique_ptr<Clause> clause = ClauseDirector::makeWithClause(builder, singleClause, declarations);
    withClauses.push_back(std::move(clause));
  }
  return withClauses;
}

