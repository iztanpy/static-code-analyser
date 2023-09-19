#include <string>
#include <memory>
#include <utility>
#include "qps/query_parser/query_parser.h"
#include "qps/query_parser/clause_builder/clause_director.h"
#include "qps/query_parser/clause_builder/suchthat_clause_builder.h"
#include "qps/query_parser/clause_builder/pattern_clause_builder.h"

ParsedQuery QueryParser::ParseTokenizedQuery(const std::vector<QueryToken> & tokens) {
  /* TODO: validates the tokens */
  ParsedQuery parsedQuery;
  // from the tokens, identify the declarations together with its synonyms
  std::vector<Declaration> declarations = ExtractDeclarations(tokens);
  // from the tokens, look for select/ such that/ pattern clauses and call the builders
  std::vector<QueryToken> selectTokens = ExtractSelectTokens(tokens);
  std::vector<SelectClause> selectClauses = ExtractSelectClauses(selectTokens, declarations);
  /* For now, we only have one synonym for select clause
   * hence we can just return the first value of the vector
   * - open to extensions in the future
   */
  if (selectClauses.size() == 1) {
    parsedQuery.select = selectClauses[0];
  }

  std::vector<QueryToken> suchThatTokens = ExtractSuchThatTokens(tokens);
  if (!suchThatTokens.empty()) {
    std::vector<std::unique_ptr<SuchThatClause>> suchThatClauses = ExtractSuchThatClauses(suchThatTokens, declarations);
    parsedQuery.such_that_clauses = std::move(suchThatClauses);
  }
  /* TODO: extract pattern clauses */
  std::vector<QueryToken> patternTokens = ExtractPatternTokens(tokens);
  if (!suchThatTokens.empty()) {
    std::vector<std::unique_ptr<PatternClause>> patternClauses = ExtractPatternClauses(patternTokens, declarations);
    parsedQuery.pattern_clauses = std::move(patternClauses);
  }
  return parsedQuery;
}

std::vector<Declaration> QueryParser::ExtractDeclarations(const std::vector<QueryToken> & tokens) {
  PQLTokenType previous_token_type;
  DesignEntity current_entity_type;
  std::vector<Declaration> declarations;
  for (const QueryToken & token : tokens) {
    switch (token.type) {
      case PQLTokenType::DECLARATION:current_entity_type = Entity::fromString(token.text);
        previous_token_type = token.type;
        break;
      case PQLTokenType::SYNONYM:
        if (previous_token_type == PQLTokenType::DECLARATION) {
          std::string synonym = token.text;
          Declaration new_declaration = {synonym, current_entity_type};
          declarations.push_back(new_declaration);
        }
        break;
      default:previous_token_type = token.type;
    }
  }
  return declarations;
}

std::vector<QueryToken> QueryParser::ExtractSelectTokens(const std::vector<QueryToken> & tokens) {
  bool inSelectClause = false;
  std::vector<QueryToken> selectTokens;
  for (const QueryToken & token : tokens) {
    switch (token.type) {
      case PQLTokenType::SELECT:inSelectClause = true;
        break;
      case PQLTokenType::SYNONYM:
        if (inSelectClause) {
          selectTokens.push_back(token);
        }
        break;
      default:inSelectClause = false;
    }
  }
  return selectTokens;
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

std::vector<QueryToken> QueryParser::ExtractSuchThatTokens(const std::vector<QueryToken> & tokens) {
  std::vector<QueryToken> suchThatTokens;
  bool inSuchThatClause = false;
  for (const QueryToken & token : tokens) {
    switch (token.type) {
      case PQLTokenType::SUCH_THAT:inSuchThatClause = true;
        break;
      case PQLTokenType::RELREF:
      case PQLTokenType::INTEGER:
      case PQLTokenType::SYNONYM:
      case PQLTokenType::IDENT:
      case PQLTokenType::WILDCARD:
        if (inSuchThatClause) {
          suchThatTokens.push_back(token);
        }
        break;
      default:inSuchThatClause = false;
    }
  }
  return suchThatTokens;
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
std::vector<QueryToken> QueryParser::ExtractPatternTokens(const std::vector<QueryToken> & tokens) {
  std::vector<QueryToken> patternTokens;
  bool inPatternClause = false;
  for (const QueryToken & token : tokens) {
    switch (token.type) {
      case PQLTokenType::PATTERN:inPatternClause = true;
        break;
      case PQLTokenType::SYNONYM:
      case PQLTokenType::IDENT:
      case PQLTokenType::WILDCARD:
      case PQLTokenType::PARTIALEXPR:
        if (inPatternClause) {
          patternTokens.push_back(token);
        }
        break;
      default:inPatternClause = false;
    }
  }

  return patternTokens;
}
std::vector<std::unique_ptr<PatternClause>> QueryParser::ExtractPatternClauses(const std::vector<QueryToken> & patternTokens,
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

