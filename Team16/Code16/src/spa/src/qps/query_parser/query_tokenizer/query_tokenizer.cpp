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
#include "qps/qps_validator/qps_validator.h"

QueryStructure::QueryStructure(std::vector<std::string> declaration_statements, std::string select_statement)
    : declaration_statements(std::move(declaration_statements)), select_statement(std::move(select_statement)) {}

QueryStructure QueryTokenizer::splitQuery(std::string sanitized_query) {
  std::vector<std::string> statements = string_util::SplitStringBy(qps_constants::kSemicolon, sanitized_query);
  std::vector<std::string> declaration_statements;
  std::string select_statement;
  // indicator to ensure we only have 1 select statement and nothing else at the back
  bool is_select_statement_processed = false;

  for (std::string statement : statements) {
    qps_validator::ValidateStatement(statement, is_select_statement_processed);
    std::string first_word = string_util::GetFirstWord(statement);
    if (first_word == qps_constants::kSelectKeyword) {
      select_statement = statement;
      is_select_statement_processed = true;
    } else {
      declaration_statements.push_back(statement);
    }
  }
  qps_validator::ValidateSelectExists(select_statement);

  return {declaration_statements, select_statement};
}

std::vector<Declaration> QueryTokenizer::extractDeclarations(const std::vector<std::string> & declaration_statements) {
  std::vector<Declaration> declarations;
  std::set < std::string > processed_synonyms;
  for (std::string statement : declaration_statements) {
    qps_validator::ValidateDeclarationStatement(statement);

    std::string entity = string_util::GetFirstWord(statement);
    DesignEntity design_entity = Entity::fromString(entity);
    statement = string_util::RemoveFirstWord(statement);
    std::vector<std::string> synonyms = string_util::SplitStringBy(',', statement);

    for (const std::string & synonym : synonyms) {
      qps_validator::ValidateDeclarationSynonym(synonym, processed_synonyms);
      processed_synonyms.insert(synonym);
      // insert each synonym into declarations
      declarations.push_back({synonym, design_entity});
    }
  }

  return declarations;
}

std::vector<QueryToken> QueryTokenizer::extractSelectToken(std::string & select_statement,
                                                           std::vector<Declaration> & declarations) {
  std::vector<QueryToken> select_tokens;
  qps_validator::ValidateSelectStatement(select_statement);
  // remove the Select keyword
  std::string remaining_statement = string_util::RemoveFirstWord(select_statement);
  // then extract the synonym after 'Select'
  std::string select_synonym = string_util::GetFirstWord(remaining_statement);
  qps_validator::ValidateSelectSynonym(select_synonym, declarations);
  select_tokens.push_back({select_synonym, PQLTokenType::SYNONYM});
  return select_tokens;
}

std::vector<size_t> QueryTokenizer::getClauseIndexes(const std::string & remaining_statement) {
  std::vector<size_t> indexes;
  std::vector<std::regex> rgxVector = {
      qps_constants::kSuchThatClauseRegex,
      qps_constants::kPatternClauseRegex,
      qps_constants::kOnlySuchThat,
      qps_constants::kOnlyPattern
  };

  for (const auto& rgx : rgxVector) {
    // Create an iterator that will search for matches in the input string
    std::sregex_iterator it(remaining_statement.begin(), remaining_statement.end(), rgx);
    std::sregex_iterator end;

    // Iterate over matches and store their starting positions
    while (it != end) {
      if (std::find(indexes.begin(), indexes.end(), it ->position()) == indexes.end()) {
        indexes.push_back(it->position());
      }
      ++it;
    }
  }

  sort(indexes.begin(), indexes.end());
  qps_validator::ValidateClauseIndexes(indexes);

  return indexes;
}

bool QueryTokenizer::clauseMatch(std::string & clause, const std::regex & regexPattern) {
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
    throw QpsSyntaxError("More than 2 open brackets or missing arguments");
  }
  std::vector<std::string> rhs = string_util::SplitStringBy(')', synonyms[1]);
  if (rhs.size() != 2) {
    throw QpsSyntaxError("More than 2 close brackets or missing arguments");
  }

  QueryToken right_token;
  QueryToken left_token;
  std::string right_hand_side = string_util::Trim(rhs[0]);
  std::string left_hand_side = string_util::Trim(lhs[1]);
  // check if lhs and rhs is a stmtRef or entRef
  if (!QueryUtil::IsStmtRef(left_hand_side) && !QueryUtil::IsEntRef(left_hand_side)) {
    throw QpsSyntaxError("Invalid argument for relationship reference");
  }
  if (!QueryUtil::IsStmtRef(right_hand_side) && !QueryUtil::IsEntRef(right_hand_side)) {
    throw QpsSyntaxError("Invalid argument for relationship reference");
  }

  // Set the different types of tokens
  if (QueryUtil::IsWildcard(left_hand_side)) {
    left_token = {left_hand_side, PQLTokenType::WILDCARD};
  } else if (lexical_utils::IsInteger(left_hand_side)) {
    left_token = {left_hand_side, PQLTokenType::INTEGER};
  } else if (QueryUtil::IsIdentWithDoubleQuotes(left_hand_side)) {
    std::string remove_quotations = QueryUtil::RemoveQuotations(left_hand_side);
    left_token = {remove_quotations, PQLTokenType::IDENT};
  } else {
    if (!QueryUtil::IsInDeclarations(left_hand_side, declarations)) {
      throw QpsSemanticError("LHS synonym not declared");
    }
    left_token = {left_hand_side, PQLTokenType::SYNONYM};
  }

  if (QueryUtil::IsWildcard(right_hand_side)) {
    right_token = {right_hand_side, PQLTokenType::WILDCARD};
  } else if (lexical_utils::IsInteger(right_hand_side)) {
    right_token = {right_hand_side, PQLTokenType::INTEGER};
  } else if (QueryUtil::IsIdentWithDoubleQuotes(right_hand_side)) {
    std::string remove_quotations = QueryUtil::RemoveQuotations(right_hand_side);
    right_token = {remove_quotations, PQLTokenType::IDENT};
  } else {
    if (!QueryUtil::IsInDeclarations(right_hand_side, declarations)) {
      throw QpsSemanticError("RHS synonym not declared");
    }
    right_token = {right_hand_side, PQLTokenType::SYNONYM};
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
    throw QpsSyntaxError("More than 2 open brackets or missing arguments");
  }
  std::vector<std::string> rhs = string_util::SplitStringBy(')', arguments[1]);
  if (rhs.size() != 2) {
    throw QpsSyntaxError("More than 2 close brackets or missing arguments");
  }

  QueryToken right_token;
  QueryToken left_token;
  std::string right_hand_side = string_util::Trim(rhs[0]);
  std::string left_hand_side = string_util::Trim(lhs[1]);
  // check if lhs is a entRef and lhs is an expr or a partial expr
  if (!QueryUtil::IsEntRef(left_hand_side)) {
    throw QpsSyntaxError("Invalid argument for LHS pattern clause");
  }
  if (!QueryUtil::IsPartialMatchExpressionSpecification(right_hand_side)
      && !QueryUtil::IsExactExpressionSpecification(right_hand_side)
      && !QueryUtil::IsWildcard(right_hand_side)) {
    throw QpsSyntaxError("Invalid argument for RHS of pattern clause");
  }

  // Set the different types of tokens
  if (QueryUtil::IsWildcard(left_hand_side)) {
    left_token = {left_hand_side, PQLTokenType::WILDCARD};
  } else if (QueryUtil::IsIdentWithDoubleQuotes(left_hand_side)) {
    std::string remove_quotations = QueryUtil::RemoveQuotations(left_hand_side);
    left_token = {remove_quotations, PQLTokenType::IDENT};
  } else {
    if (!QueryUtil::IsInDeclarations(left_hand_side, declarations)) {
      throw QpsSemanticError("LHS synonym not declared");
    }
    left_token = {left_hand_side, PQLTokenType::SYNONYM};
  }

  if (QueryUtil::IsWildcard(right_hand_side)) {
    right_token = {right_hand_side, PQLTokenType::WILDCARD};
  } else if (QueryUtil::IsExactExpressionSpecification(right_hand_side)) {
    std::string remove_quotations = QueryUtil::RemoveQuotations(right_hand_side);
    right_token = {remove_quotations, PQLTokenType::IDENT};
  } else {
    std::string remove_wildcard = string_util::Trim(right_hand_side.substr(1, right_hand_side.length() - 2));
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

      if (clause_with_such_that_removed.empty()) {
        throw QpsSyntaxError("Missing input after such that");
      }

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
      if (clause_with_pattern_removed.empty()) {
        throw QpsSyntaxError("Missing input after pattern");
      }

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

  std::vector<Declaration>
      declarations = QueryTokenizer::extractDeclarations(statements.declaration_statements);
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
