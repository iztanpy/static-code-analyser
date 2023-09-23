#include <string>
#include <algorithm>
#include <utility>
#include <vector>
#include <set>
#include <regex>
#include "qps/query_parser/query_tokenizer/query_tokenizer.h"
#include "qps/design_entity.h"
#include "utils/string_utils.h"
#include "utils/lexical_utils.h"
#include "qps/qps_errors/qps_syntax_error.h"
#include "qps/qps_errors/qps_semantic_error.h"
#include "qps/constants.h"
#include "qps/query_parser/QueryUtil.h"

QueryStructure::QueryStructure(std::vector<std::string> declaration_statements, std::string select_statement)
    : declaration_statements(std::move(declaration_statements)), select_statement(std::move(select_statement)) {}

QueryStructure QueryTokenizer::splitQuery(std::string sanitized_query) {
  std::vector<std::string> statements = string_util::SplitStringBy(';', sanitized_query);
  std::vector<std::string> declaration_statements;
  std::string select_statement;
  // indicator to ensure we only have 1 select statement and nothing else at the back
  bool is_select_statement_processed = false;

  for (std::string statement : statements) {
    if (is_select_statement_processed) {
      throw QpsSyntaxError("Statements after select statement are invalid");
    }
    std::string first_word = string_util::GetFirstWord(statement);
    if (first_word == "Select") {
      select_statement = statement;
      is_select_statement_processed = true;
    } else {
      declaration_statements.push_back(statement);
    }
  }

  if (select_statement.empty()) {
    throw QpsSyntaxError("No select statement");
  }
  return {declaration_statements, select_statement};
}

std::vector<Declaration> QueryTokenizer::extractDeclarations(const std::vector<std::string> & declaration_statements) {
  std::set < std::string > stringDesignEntities = Entity::getStringDesignEntities();
  std::vector<Declaration> declarations;
  for (std::string statement : declaration_statements) {
    std::string entity = string_util::GetFirstWord(statement);

    if (!QueryUtil::IsDesignEntity(entity)) {
      throw QpsSyntaxError("Design entity is not valid");
    }

    DesignEntity design_entity = Entity::fromString(entity);
    // We can ignore the entity already
    statement = string_util::RemoveFirstWord(statement);
    if (statement.empty()) {
      throw QpsSyntaxError("Missing synonyms for design entity");
    }

    std::set < std::string > processed_synonyms;
    std::vector<std::string> synonyms = string_util::SplitStringBy(',', statement);
    for (const std::string & synonym : synonyms) {
      // synonym must be IDENT
      if (!lexical_utils::IsIdent(synonym)) {
        throw QpsSyntaxError("Synonym does not follow lexical rules");
      }
      if (processed_synonyms.find(synonym) != processed_synonyms.end()) {
        throw QpsSemanticError("Repeated synonym declaration");
      } else {
        processed_synonyms.insert(synonym);
      }
      // insert each synonym into declarations
      declarations.push_back({synonym, design_entity});
    }
  }
  return declarations;
}

// TODO(Su Mian): This may not need to be a vector depending on how we parse
std::vector<QueryToken> QueryTokenizer::extractSelectToken(std::string & select_statement,
                                                           const std::vector<Declaration> & declarations) {
  std::vector<QueryToken> select_tokens;
  // remove the Select keyword (guaranteed to exist since we already checked)
  std::string remaining_statement = string_util::RemoveFirstWord(select_statement);
  // then extract the first word after 'Select'
  std::string first_word = string_util::GetFirstWord(remaining_statement);
  if (!QueryUtil::IsInDeclarations(first_word, declarations)) {
    throw QpsSemanticError("Select synonym has not been declared");
  }
  // and if it has been declared, then the synonym should be syntactically correct already
  select_tokens.push_back({first_word, PQLTokenType::SYNONYM});
  return select_tokens;
}

std::vector<size_t> QueryTokenizer::getClauseIndexes(const std::string & remaining_statement) {
  std::vector<size_t> indexes;
  size_t startPos = 0;

  while (startPos < remaining_statement.size()) {
    size_t suchThatPos = remaining_statement.find("such that", startPos);
    size_t patternPos = remaining_statement.find("pattern", startPos);

    // Find the position of "such that" and "pattern" clauses
    if (suchThatPos != std::string::npos) {
      indexes.push_back(suchThatPos);  // "such that" has 9 characters
      startPos = suchThatPos + 9;  // Move the starting position past "such that"
    } else if (patternPos != std::string::npos) {
      indexes.push_back(patternPos);  // "pattern" has 7 characters
      startPos = patternPos + 7;  // Move the starting position past "pattern"
    } else {
      break;  // No more "such that" or "pattern" clauses found
    }
  }

  sort(indexes.begin(), indexes.end());

  if (indexes.empty()) {
    // the clauses are wrong syntactically
    throw QpsSyntaxError("Invalid clause expressions");
  }

  if (indexes[0] > 0) {
    // clause does not start immediately after Select clause e.g., Select v ____ such that _____
    throw QpsSyntaxError("Unexpected clause expression");
  }

  return indexes;
}

bool clauseMatch(const std::string & clause, const std::regex & regexPattern) {
  return std::regex_search(clause, regexPattern);
}

std::pair<QueryToken, QueryToken> QueryTokenizer::getRelRefArgs(std::string & clause,
                                                                const std::vector<Declaration> & declarations) {
  std::vector<std::string> synonyms = string_util::SplitStringBy(',', clause);
  if (synonyms.size() != 2) {
    throw QpsSyntaxError("More than 2 synonyms in relation reference");
  }

  std::vector<std::string> lhs = string_util::SplitStringBy('(', synonyms[0]);
  if (lhs.size() != 2) {
    throw QpsSyntaxError("More than 2 open brackets");
  }
  std::vector<std::string> rhs = string_util::SplitStringBy(')', synonyms[1]);
  if (rhs.size() != 1) {
    throw QpsSyntaxError("More than 2 close brackets");
  }

  QueryToken right_token;
  QueryToken left_token;
  // check if lhs and rhs is a stmtRef or entRef
  if (!QueryUtil::IsStmtRef(lhs[1]) && !QueryUtil::IsEntRef(lhs[1])) {
    throw QpsSyntaxError("Invalid argument for relationship reference");
  }
  if (!QueryUtil::IsStmtRef(rhs[0]) && !QueryUtil::IsEntRef(rhs[0])) {
    throw QpsSyntaxError("Invalid argument for relationship reference");
  }

  // Set the different types of tokens
  if (QueryUtil::IsWildcard(lhs[1])) {
    left_token = {lhs[1], PQLTokenType::WILDCARD};
  } else if (lexical_utils::IsInteger(lhs[1])) {
    left_token = {lhs[1], PQLTokenType::INTEGER};
  } else if (QueryUtil::IsIdentWithDoubleQuotes(lhs[1])) {
    std::string remove_quotations = QueryUtil::RemoveQuotations(lhs[1]);
    left_token = {remove_quotations, PQLTokenType::IDENT};
  } else {
    if (!QueryUtil::IsInDeclarations(lhs[1], declarations)) {
      throw QpsSemanticError("LHS synonym not declared");
    }
    left_token = {lhs[1], PQLTokenType::SYNONYM};
  }

  if (QueryUtil::IsWildcard(rhs[0])) {
    right_token = {rhs[0], PQLTokenType::WILDCARD};
  } else if (lexical_utils::IsInteger(rhs[0])) {
    right_token = {rhs[0], PQLTokenType::INTEGER};
  } else if (QueryUtil::IsIdentWithDoubleQuotes(rhs[0])) {
    std::string remove_quotations = QueryUtil::RemoveQuotations(rhs[0]);
    right_token = {remove_quotations, PQLTokenType::IDENT};
  } else {
    if (!QueryUtil::IsInDeclarations(rhs[0], declarations)) {
      throw QpsSemanticError("RHS synonym not declared");
    }
    right_token = {rhs[0], PQLTokenType::SYNONYM};
  }

  std::pair<QueryToken, QueryToken> syn_pair = {left_token, right_token};
  return syn_pair;
}

std::pair<QueryToken, QueryToken> QueryTokenizer::getPatternArgs(std::string & clause,
                                                                 const std::vector<Declaration> & declarations) {
  std::vector<std::string> arguments = string_util::SplitStringBy(',', clause);
  if (arguments.size() != 2) {
    throw QpsSyntaxError("More than 2 arguments in pattern clause");
  }

  std::vector<std::string> lhs = string_util::SplitStringBy('(', arguments[0]);
  if (lhs.size() != 2) {
    throw QpsSyntaxError("More than 2 open brackets");
  }
  std::vector<std::string> rhs = string_util::SplitStringBy(')', arguments[1]);
  if (rhs.size() != 1) {
    throw QpsSyntaxError("More than 2 close brackets");
  }

  QueryToken right_token;
  QueryToken left_token;
  // check if lhs is a entRef and lhs is an expr or a partial expr
  if (!QueryUtil::IsEntRef(lhs[1])) {
    throw QpsSyntaxError("Invalid argument for LHS pattern clause");
  }
  if (!QueryUtil::IsPartialMatchExpressionSpecification(rhs[0]) && !QueryUtil::IsIdentWithDoubleQuotes(rhs[0])
      && !QueryUtil::IsWildcard(rhs[0])) {
    throw QpsSyntaxError("Invalid argument for RHS of pattern clause");
  }

  // Set the different types of tokens
  if (QueryUtil::IsWildcard(lhs[1])) {
    left_token = {lhs[1], PQLTokenType::WILDCARD};
  } else if (QueryUtil::IsIdentWithDoubleQuotes(lhs[1])) {
    std::string remove_quotations = QueryUtil::RemoveQuotations(lhs[1]);
    left_token = {remove_quotations, PQLTokenType::IDENT};
  } else {
    if (!QueryUtil::IsInDeclarations(lhs[1], declarations)) {
      throw QpsSemanticError("LHS synonym not declared");
    }
    left_token = {lhs[1], PQLTokenType::SYNONYM};
  }

  if (QueryUtil::IsWildcard(rhs[0])) {
    right_token = {rhs[0], PQLTokenType::WILDCARD};
  } else if (QueryUtil::IsIdentWithDoubleQuotes(rhs[0])) {
    std::string remove_quotations = QueryUtil::RemoveQuotations(rhs[0]);
    right_token = {remove_quotations, PQLTokenType::IDENT};
  } else {
    std::string remove_wildcard = string_util::Trim(rhs[0].substr(1, rhs[0].length() - 2));
    std::string remove_quotations = QueryUtil::RemoveQuotations(remove_wildcard);
    right_token = {remove_quotations, PQLTokenType::PARTIALEXPR};
  }

  std::pair<QueryToken, QueryToken> arg_pair = {left_token, right_token};
  return arg_pair;
}

std::pair<std::vector<QueryToken>,
          std::vector<QueryToken>> QueryTokenizer::extractClauseTokens(std::string select_statement,
                                                                       const std::vector<Declaration> & declarations) {
  std::vector<QueryToken> such_that_tokens;
  std::vector<QueryToken> pattern_tokens;
  // remove the Select keyword (guaranteed to exist since we already checked)
  std::string remaining_statement = string_util::RemoveFirstWord(select_statement);
  // remove the Select synonym
  remaining_statement = string_util::RemoveFirstWord(remaining_statement);
  if (remaining_statement.empty()) {
    return {such_that_tokens, pattern_tokens};
  }
  std::vector<size_t> clause_beginning_indexes = getClauseIndexes(remaining_statement);
  clause_beginning_indexes.push_back(remaining_statement.length());  // push back to avoid out of range error
  std::string prev_clause;
  std::string curr_clause;
  std::string processed_clause;
  size_t start_index = 0;

  for (int i = 0; i < clause_beginning_indexes.size() - 1; i++) {
    size_t next_index = clause_beginning_indexes[i + 1];
    curr_clause = string_util::Trim(remaining_statement.substr(start_index, next_index - start_index));
    start_index = next_index;

    if (clauseMatch(curr_clause, qps_constants::kSuchThatClauseRegex)) {
      std::string clause_with_such_that_removed = string_util::RemoveFirstWord(curr_clause);
      clause_with_such_that_removed = string_util::RemoveFirstWord(clause_with_such_that_removed);
      std::string rel_ref = string_util::GetFirstWordFromArgs(clause_with_such_that_removed);

      if (QueryUtil::IsRelRef(rel_ref)) {
        such_that_tokens.push_back({rel_ref, PQLTokenType::RELREF});
      } else {
        throw QpsSyntaxError("Invalid relation reference type");
      }

      std::string rel_ref_syn_pair = string_util::RemoveFirstWordFromArgs(clause_with_such_that_removed);
      std::pair<QueryToken, QueryToken> rel_ref_args = getRelRefArgs(rel_ref_syn_pair, declarations);
      such_that_tokens.push_back(rel_ref_args.first);
      such_that_tokens.push_back(rel_ref_args.second);
    } else if (clauseMatch(curr_clause, qps_constants::kPatternClauseRegex)) {
      std::string clause_with_pattern_removed = string_util::RemoveFirstWord(curr_clause);
      std::string syn_assign = string_util::GetFirstWordFromArgs(clause_with_pattern_removed);
      if (QueryUtil::IsSynonym(syn_assign)) {
        pattern_tokens.push_back({syn_assign, PQLTokenType::SYNONYM});
      } else {
        throw QpsSemanticError("Synonym assign is not declared");
      }

      std::string pattern_arg_pair = string_util::RemoveFirstWordFromArgs(clause_with_pattern_removed);
      std::pair<QueryToken, QueryToken> pattern_args = getPatternArgs(pattern_arg_pair, declarations);
      pattern_tokens.push_back(pattern_args.first);
      pattern_tokens.push_back(pattern_args.second);
    } else {
      throw QpsSyntaxError("Unrecognized clause");
    }
  }

  return {such_that_tokens, pattern_tokens};
}

TokenisedQuery QueryTokenizer::tokenize(const std::string & query) {
  std::vector<QueryToken> tokens;
  std::string sanitized_query = string_util::RemoveWhiteSpaces(query);
  QueryStructure statements = QueryTokenizer::splitQuery(sanitized_query);
  std::vector<Declaration> declarations = QueryTokenizer::extractDeclarations(statements.declaration_statements);
  std::vector<QueryToken>
      select_tokens = QueryTokenizer::extractSelectToken(statements.select_statement, declarations);
  std::pair<std::vector<QueryToken>, std::vector<QueryToken>>
      clause_tokens = QueryTokenizer::extractClauseTokens(statements.select_statement, declarations);
  std::vector<QueryToken> such_that_tokens = clause_tokens.first;
  std::vector<QueryToken> pattern_tokens = clause_tokens.second;

  return {
      declarations,
      select_tokens,
      such_that_tokens,
      pattern_tokens
  };
}
