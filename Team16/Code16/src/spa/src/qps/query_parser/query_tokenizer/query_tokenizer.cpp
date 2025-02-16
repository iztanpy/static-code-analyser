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

std::vector<Declaration> QueryTokenizer::extractDeclarations(const std::vector<std::string>& declaration_statements) {
  if (semantic_errors.empty()) {
    // prevent segmentation fault
    semantic_errors ={};
  }
  std::vector<Declaration> declarations;
  std::set<std::string> processed_synonyms;
  for (std::string statement : declaration_statements) {
    qps_validator::ValidateDeclarationStatement(statement);

    std::string entity = string_util::GetFirstWord(statement);
    DesignEntity design_entity = Entity::fromString(entity);
    statement = string_util::RemoveFirstWord(statement);
    std::vector<std::string> synonyms = string_util::SplitStringBy(',', statement);

    for (const std::string& synonym : synonyms) {
      qps_validator::ValidateDeclarationSynonym(synonym, processed_synonyms, semantic_errors);
      processed_synonyms.insert(synonym);
      // insert each synonym into declarations
      declarations.push_back({synonym, design_entity});
    }
  }

  return declarations;
}

size_t QueryTokenizer::getFirstClauseIndexes(const std::string& remaining_statement) {
  std::vector<size_t> indexes;
  std::vector<std::regex> rgxVector = {
      qps_constants::kSuchThatClauseRegex,
      qps_constants::kPatternClauseRegex,
      qps_constants::kAndClauseRegex,
      qps_constants::kWithClauseRegex,
      qps_constants::kOnlySuchThat,
      qps_constants::kOnlyPattern,
      qps_constants::kOnlyAnd,
      qps_constants::kOnlyWith
  };

  for (const auto& rgx : rgxVector) {
    // Create an iterator that will search for matches in the input string
    std::sregex_iterator it(remaining_statement.begin(), remaining_statement.end(), rgx);
    std::sregex_iterator end;

    // Iterate over matches and store their starting positions
    while (it != end) {
      if (std::find(indexes.begin(), indexes.end(), it->position()) == indexes.end()) {
        indexes.push_back(it->position());
      }
      ++it;
    }
  }
  indexes.push_back(remaining_statement.length());
  sort(indexes.begin(), indexes.end());

  return indexes[0];
}

SelectValueType QueryTokenizer::getSelectValueType(std::string& select_value) {
  if (QueryUtil::IsSynonym(select_value) || QueryUtil::IsAttrRef(select_value)) {
    return SelectValueType::SINGLE;
  } else if (QueryUtil::IsEnclosedInTuple(select_value)) {
    return SelectValueType::MUTLIPLE;
  } else {
    throw QpsSyntaxError("Invalid select clause");
  }
}

std::vector<QueryToken> processSelectClause(std::string& select_statement, SelectValueType select_value_type) {
  std::vector<QueryToken> results;
  std::string select_with_tuple_removed;
  std::vector<std::string> tuple_arguments;
  switch (select_value_type) {
    case SelectValueType::SINGLE:
      if (lexical_utils::IsSynonym(select_statement)) {
        results.push_back({select_statement, PQLTokenType::SYNONYM});
      } else if (QueryUtil::IsAttrRef(select_statement)) {
        std::vector<std::string> split_attr_ref = string_util::SplitStringBy('.', select_statement);
        std::string attr_syn = split_attr_ref[0];
        std::string attr_name = split_attr_ref[1];
        PQLTokenType token_type = QueryTokenizer::getAttrTokenType(attr_name);
        results.push_back({attr_syn, token_type});
      }
      break;
    case SelectValueType::MUTLIPLE:
      select_with_tuple_removed = QueryUtil::RemoveTuple(select_statement);
      tuple_arguments = string_util::SplitStringBy(',', select_with_tuple_removed);
      for (std::string& tuple_argument : tuple_arguments) {
        if (lexical_utils::IsSynonym(tuple_argument)) {
          results.push_back({tuple_argument, PQLTokenType::SYNONYM});
        } else if (QueryUtil::IsAttrRef(tuple_argument)) {
          std::vector<std::string> split_attr_ref = string_util::SplitStringBy('.', tuple_argument);
          std::string attr_syn = split_attr_ref[0];
          std::string attr_name = split_attr_ref[1];
          PQLTokenType token_type = QueryTokenizer::getAttrTokenType(attr_name);
          results.push_back({attr_syn, token_type});
        }
      }
      break;
    default:
      throw QpsSyntaxError("Invalid select clause");
  }
  return results;
}

std::string QueryTokenizer::removeResultClause(std::string& select_statement) {
  std::string modifiedQuery = select_statement;
  if (std::regex_search(select_statement, qps_constants::kTupleRegex)) {
    // Replace the result-cl with an empty string
    modifiedQuery = std::regex_replace(select_statement, qps_constants::kTupleRegex, "");
  } else {
    // either a BOOLEAN or synonym or attrRef
    modifiedQuery = string_util::RemoveFirstWord(select_statement);
  }
  // Remove any extra spaces or whitespace characters
  modifiedQuery = std::regex_replace(modifiedQuery, std::regex("\\s+"), " ");
  return modifiedQuery;
}

std::string QueryTokenizer::removeAfterResultClause(std::string& select_statement) {
  std::string preservedResultClause = select_statement;
  std::smatch match;
  if (std::regex_search(select_statement, match, qps_constants::kTupleRegex)) {
    preservedResultClause = match[0];
  } else {
    // either a BOOLEAN or synonym
    preservedResultClause = string_util::GetFirstWord(select_statement);
  }
  // Remove any extra spaces or whitespace characters
  preservedResultClause = std::regex_replace(preservedResultClause, std::regex("\\s+"), " ");
  return preservedResultClause;
}

std::vector<QueryToken> QueryTokenizer::extractSelectToken(std::string& select_statement,
                                                           std::vector<Declaration>& declarations) {
  if (semantic_errors.empty()) {
    // prevent segmentation fault
    semantic_errors = {};
  }

  std::vector<QueryToken> select_tokens;
  qps_validator::ValidateSelectStatement(select_statement);
  // remove the Select keyword
  std::string remaining_statement = string_util::RemoveFirstWord(select_statement);
  // get the remaining statement and validate it
  std::string statement_after_res_cl = QueryTokenizer::removeResultClause(remaining_statement);
  qps_validator::ValidateStatementAfterResClause(statement_after_res_cl);
  std::string select_value = QueryTokenizer::removeAfterResultClause(remaining_statement);
  SelectValueType select_value_type = getSelectValueType(select_value);
  qps_validator::ValidateSelectValue(select_value, select_value_type, declarations, semantic_errors);
  // then extract the synonym after 'Select'
  std::vector<QueryToken> select_query_tokens = processSelectClause(select_value, select_value_type);
  for (const QueryToken& token : select_query_tokens) {
    select_tokens.push_back(token);
  }
  return select_tokens;
}

std::vector<size_t> QueryTokenizer::getClauseIndexes(const std::string& remaining_statement) {
  std::vector<size_t> indexes;
  std::vector<std::regex> rgxVector = {
      qps_constants::kSuchThatClauseRegex,
      qps_constants::kPatternClauseRegex,
      qps_constants::kAndClauseRegex,
      qps_constants::kWithClauseRegex,
      qps_constants::kOnlySuchThat,
      qps_constants::kOnlyPattern,
      qps_constants::kOnlyAnd,
      qps_constants::kOnlyWith
  };

  for (const auto& rgx : rgxVector) {
    // Create an iterator that will search for matches in the input string
    std::sregex_iterator it(remaining_statement.begin(), remaining_statement.end(), rgx);
    std::sregex_iterator end;

    // Iterate over matches and store their starting positions
    while (it != end) {
      if (std::find(indexes.begin(), indexes.end(), it->position()) == indexes.end()) {
        indexes.push_back(it->position());
      }
      ++it;
    }
  }

  sort(indexes.begin(), indexes.end());
  qps_validator::ValidateClauseIndexes(indexes);

  return indexes;
}

bool QueryTokenizer::clauseMatch(std::string& clause, const std::regex& regexPattern) {
  return std::regex_search(clause, regexPattern);
}

std::pair<QueryToken, QueryToken> QueryTokenizer::getRelRefArgs(std::string& clause,
                                                                std::vector<Declaration>& declarations) {
  qps_validator::ValidateClauseArgs(clause);
  clause = QueryUtil::RemoveBrackets(clause);
  std::vector<std::string> synonyms = string_util::SplitStringBy(',', clause);

  QueryToken right_token;
  QueryToken left_token;
  std::string right_hand_side = string_util::Trim(synonyms[1]);
  std::string left_hand_side = string_util::Trim(synonyms[0]);
  // check if lhs and rhs is a stmtRef or entRef
  qps_validator::ValidateClauseArgs(left_hand_side, right_hand_side);

  // Set the different types of tokens
  if (QueryUtil::IsWildcard(left_hand_side)) {
    left_token = {left_hand_side, PQLTokenType::WILDCARD};
  } else if (lexical_utils::IsInteger(left_hand_side)) {
    left_token = {left_hand_side, PQLTokenType::INTEGER};
  } else if (QueryUtil::IsIdentWithDoubleQuotes(left_hand_side)) {
    std::string remove_quotations = QueryUtil::RemoveQuotations(left_hand_side);
    left_token = {remove_quotations, PQLTokenType::IDENT};
  } else {
    qps_validator::ValidateClauseSynonym(left_hand_side, declarations, semantic_errors);
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
    qps_validator::ValidateClauseSynonym(right_hand_side, declarations, semantic_errors);
    right_token = {right_hand_side, PQLTokenType::SYNONYM};
  }

  std::pair<QueryToken, QueryToken> syn_pair = {left_token, right_token};
  return syn_pair;
}

std::pair<QueryToken, QueryToken> QueryTokenizer::getPatternArgs(std::string& clause,
                                                                 std::vector<Declaration>& declarations,
                                                                 PQLTokenType pattern_type) {
  qps_validator::ValidateClauseArgs(clause, pattern_type);
  std::string clause_with_brackets_removed = QueryUtil::RemoveBrackets(clause);
  std::vector<std::string> arguments = string_util::SplitStringBy(',', clause_with_brackets_removed);

  QueryToken right_token;
  QueryToken left_token;
  std::string left_hand_side = string_util::RemoveSpacesFromExpr(arguments[0]);
  std::string right_hand_side = string_util::RemoveSpacesFromExpr(arguments[1]);
  // check if lhs is a entRef and lhs is valid for each pattern type
  qps_validator::ValidatePatternClauseArgs(left_hand_side, right_hand_side, pattern_type, semantic_errors);
  // Additional check for third argument of if pattern
  if (pattern_type == PQLTokenType::PATTERN_IF) {
    qps_validator::ValidateIfPatternClause(arguments, semantic_errors);
  }

  // Set the different types of tokens
  if (QueryUtil::IsWildcard(left_hand_side)) {
    left_token = {left_hand_side, PQLTokenType::WILDCARD};
  } else if (QueryUtil::IsIdentWithDoubleQuotes(left_hand_side)) {
    std::string remove_quotations = QueryUtil::RemoveQuotations(left_hand_side);
    left_token = {remove_quotations, PQLTokenType::IDENT};
  } else {
    qps_validator::ValidateClauseSynonym(left_hand_side, declarations, semantic_errors);
    left_token = {left_hand_side, PQLTokenType::SYNONYM};
  }

  if (QueryUtil::IsWildcard(right_hand_side)) {
    right_token = {right_hand_side, PQLTokenType::WILDCARD};
  } else if (QueryUtil::IsExactExpressionSpecification(string_util::RemoveSpacesFromExpr(right_hand_side))) {
    std::string remove_quotations = QueryUtil::RemoveQuotations(right_hand_side);
    remove_quotations = QueryUtil::addParentheses(remove_quotations);
    right_token = {remove_quotations, PQLTokenType::EXACTEXPR};
  } else {
    std::string remove_wildcard = string_util::Trim(right_hand_side.substr(1, right_hand_side.length() - 2));
    std::string remove_quotations = QueryUtil::RemoveQuotations(remove_wildcard);
    remove_quotations = QueryUtil::addParentheses(remove_quotations);
    right_token = {remove_quotations, PQLTokenType::PARTIALEXPR};
  }

  std::pair<QueryToken, QueryToken> arg_pair = {left_token, right_token};
  return arg_pair;
}

PQLTokenType QueryTokenizer::getAttrTokenType(const std::string & attrName) {
  if (attrName == "procName") {
    return PQLTokenType::WITH_PROCNAME;
  } else if (attrName == "varName") {
    return PQLTokenType::WITH_VARNAME;
  } else if (attrName == "value") {
    return PQLTokenType::WITH_VALUE;
  } else {
    return PQLTokenType::WITH_STMTNO;
  }
}

std::pair<QueryToken, QueryToken> QueryTokenizer::getWithArgs(std::string& clause,
                                                              std::vector<Declaration>& declarations) {
  qps_validator::ValidateWithClauseArgs(clause);
  std::vector<std::string> refs = string_util::SplitStringBy('=', clause);

  QueryToken right_token;
  QueryToken left_token;
  std::string right_hand_side = string_util::Trim(refs[1]);
  std::string left_hand_side = string_util::Trim(refs[0]);

  // Set the different types of tokens
  if (QueryUtil::IsIdentWithDoubleQuotes(left_hand_side)) {
    std::string remove_quotations = QueryUtil::RemoveQuotations(left_hand_side);
    left_token = {remove_quotations, PQLTokenType::IDENT};
  } else if (lexical_utils::IsInteger(left_hand_side)) {
    left_token = {left_hand_side, PQLTokenType::INTEGER};
  } else {
    // Is AttrRef
    std::vector<std::string> attrRef_split = string_util::SplitStringBy('.', left_hand_side);
    std::string syn_string = attrRef_split[0];
    std::string attrName_string = attrRef_split[1];
    qps_validator::ValidateClauseSynonym(syn_string, declarations, semantic_errors);
    PQLTokenType token_type = getAttrTokenType(attrName_string);
    left_token = {syn_string, token_type};
  }

  if (QueryUtil::IsIdentWithDoubleQuotes(right_hand_side)) {
    std::string remove_quotations = QueryUtil::RemoveQuotations(right_hand_side);
    right_token = {remove_quotations, PQLTokenType::IDENT};
  } else if (lexical_utils::IsInteger(right_hand_side)) {
    right_token = {right_hand_side, PQLTokenType::INTEGER};
  } else {
    // Is AttrRef
    std::vector<std::string> attrRef_split = string_util::SplitStringBy('.', right_hand_side);
    std::string syn_string = attrRef_split[0];
    std::string attrName_string = attrRef_split[1];
    qps_validator::ValidateClauseSynonym(syn_string, declarations, semantic_errors);
    PQLTokenType token_type = getAttrTokenType(attrName_string);
    qps_validator::ValidateAttributeRef(syn_string, attrName_string, declarations, semantic_errors);
    right_token = {syn_string, token_type};
  }

  std::pair<QueryToken, QueryToken> syn_pair = {left_token, right_token};
  return syn_pair;
}

PQLTokenType QueryTokenizer::getPatternTokenType(std::string & pattern_syn, std::vector<Declaration>& declarations) {
  Declaration synonym_declaration;
  for (Declaration declaration : declarations) {
    if (declaration.synonym == pattern_syn) {
      synonym_declaration = declaration;
      break;
    }
  }
  switch (synonym_declaration.design_entity) {
    case DesignEntity::WHILE_LOOP: return PQLTokenType::PATTERN_WHILE;
    case DesignEntity::IF_STMT: return PQLTokenType::PATTERN_IF;
    case DesignEntity::ASSIGN:
    default:
      return PQLTokenType::SYNONYM;
  }
}

QueryToken processNot(std::string clause) {
  // checks whether the first word is 'not'
  std::string first_word = string_util::GetFirstWord(clause);
  if (first_word == qps_constants::kNot) {
    return {qps_constants::kNot, PQLTokenType::NOT_CLAUSE};
  } else {
    return {"", PQLTokenType::NORMAL_CLAUSE};
  }
}

std::vector<QueryToken> QueryTokenizer::processSuchThatClause(std::string clause_with_such_that_removed,
                                              std::vector<Declaration>& declarations) {
  std::vector<QueryToken> result;
  QueryToken not_query_token = processNot(clause_with_such_that_removed);
  result.push_back(not_query_token);
  if (not_query_token.type == PQLTokenType::NOT_CLAUSE) {
    // remove 'not'
    clause_with_such_that_removed = string_util::RemoveFirstWord(clause_with_such_that_removed);
  }
  qps_validator::ValidateNonEmptyClause(clause_with_such_that_removed);
  std::string rel_ref = string_util::GetFirstWordFromArgs(clause_with_such_that_removed);
  qps_validator::ValidateRelRef(rel_ref);
  result.push_back({rel_ref, PQLTokenType::RELREF});

  std::string rel_ref_syn_pair = string_util::RemoveFirstWordFromArgs(clause_with_such_that_removed);
  std::pair<QueryToken, QueryToken> rel_ref_args = getRelRefArgs(rel_ref_syn_pair, declarations);
  result.push_back(rel_ref_args.first);
  result.push_back(rel_ref_args.second);
  return result;
}

std::vector<QueryToken> QueryTokenizer::processPatternClause(std::string clause_with_pattern_removed,
                                             std::vector<Declaration>& declarations) {
  std::vector<QueryToken> result;
  QueryToken not_query_token = processNot(clause_with_pattern_removed);
  result.push_back(not_query_token);
  if (not_query_token.type == PQLTokenType::NOT_CLAUSE) {
    // remove 'not'
    clause_with_pattern_removed = string_util::RemoveFirstWord(clause_with_pattern_removed);
  }
  qps_validator::ValidateNonEmptyClause(clause_with_pattern_removed);
  std::string pattern_syn = string_util::GetFirstWordFromArgs(clause_with_pattern_removed);
  PQLTokenType pattern_type = getPatternTokenType(pattern_syn, declarations);  // throws semantic error

  std::string pattern_arg = string_util::RemoveFirstWordFromArgs(clause_with_pattern_removed);
  std::pair<QueryToken, QueryToken> pattern_args = getPatternArgs(pattern_arg, declarations, pattern_type);
  qps_validator::ValidatePatternSynonym(pattern_syn, declarations, semantic_errors);
  result.push_back({pattern_syn, pattern_type});
  result.push_back(pattern_args.first);
  result.push_back(pattern_args.second);
  return result;
}

std::vector<QueryToken> QueryTokenizer::processWithClause(std::string clause_with_with_removed,
                                                          std::vector<Declaration>& declarations) {
  std::vector<QueryToken> result;
  QueryToken not_query_token = processNot(clause_with_with_removed);
  result.push_back(not_query_token);
  if (not_query_token.type == PQLTokenType::NOT_CLAUSE) {
    // remove 'not'
    clause_with_with_removed = string_util::RemoveFirstWord(clause_with_with_removed);
  }
  qps_validator::ValidateNonEmptyClause(clause_with_with_removed);
  std::pair<QueryToken, QueryToken> with_args = getWithArgs(clause_with_with_removed, declarations);
  result.push_back(with_args.first);
  result.push_back(with_args.second);
  return result;
}

std::string QueryTokenizer::removeSelectClause(const std::string& remaining_statement) {
  size_t first_clause_index = getFirstClauseIndexes(remaining_statement);
  std::string clauses = string_util::Trim(remaining_statement.substr(first_clause_index, remaining_statement.length()));
  return clauses;
}

std::vector<std::vector<QueryToken>> QueryTokenizer::extractClauseTokens(std::string select_statement,
                                                                       std::vector<Declaration>& declarations) {
  if (semantic_errors.empty()) {
    // prevent segmentation fault
    semantic_errors ={};
  }
  std::vector<QueryToken> such_that_tokens;
  std::vector<QueryToken> pattern_tokens;
  std::vector<QueryToken> with_tokens;
  // remove the Select keyword (guaranteed to exist since we already checked)
  std::string remaining_statement = string_util::RemoveFirstWord(select_statement);
  // remove the Select synonym
//  remaining_statement = string_util::RemoveFirstWord(remaining_statement);
  remaining_statement = string_util::Trim(QueryTokenizer::removeResultClause(remaining_statement));
  if (remaining_statement.empty()) {
    return {such_that_tokens, pattern_tokens, with_tokens};
  }

  std::vector<size_t> clause_beginning_indexes = getClauseIndexes(remaining_statement);
  clause_beginning_indexes.push_back(remaining_statement.length());  // push back to avoid out of range error

  ClauseEnum prev_clause = ClauseEnum::NONE;  // initialize prev clause to none
  std::string curr_clause;
  size_t start_index = 0;

  for (int i = 0; i < clause_beginning_indexes.size() - 1; i++) {
    size_t next_index = clause_beginning_indexes[i + 1];
    curr_clause = string_util::Trim(remaining_statement.substr(start_index, next_index - start_index));
    start_index = next_index;

    if (clauseMatch(curr_clause, qps_constants::kAndClauseRegex)) {
      std::vector<QueryToken> tokens;
      qps_validator::ValidateAndIsNotFirstClause(prev_clause);
      std::string clause_with_and_removed = string_util::RemoveFirstWord(curr_clause);
      qps_validator::ValidateAndClause(clause_with_and_removed);
      switch (prev_clause) {
        case ClauseEnum::SUCH_THAT:
          tokens = processSuchThatClause(clause_with_and_removed, declarations);
          for (const QueryToken& token : tokens) {
            such_that_tokens.push_back(token);
          }
          break;
        case ClauseEnum::PATTERN:
          tokens = processPatternClause(clause_with_and_removed, declarations);
          for (const QueryToken& token : tokens) {
            pattern_tokens.push_back(token);
          }
          break;
        case ClauseEnum::WITH:
          tokens = processWithClause(clause_with_and_removed, declarations);
          for (const QueryToken& token : tokens) {
            with_tokens.push_back(token);
          }
          break;
        default:
          throw QpsSyntaxError("Unrecognised clause");
      }
    } else if (clauseMatch(curr_clause, qps_constants::kSuchThatClauseRegex)) {
      prev_clause = ClauseEnum::SUCH_THAT;
      std::string clause_with_such_that_removed = string_util::RemoveFirstWord(curr_clause);
      clause_with_such_that_removed = string_util::RemoveFirstWord(clause_with_such_that_removed);
      std::vector<QueryToken> tokens = processSuchThatClause(clause_with_such_that_removed, declarations);
      for (const QueryToken& token : tokens) {
        such_that_tokens.push_back(token);
      }

    } else if (clauseMatch(curr_clause, qps_constants::kPatternClauseRegex)) {
      prev_clause = ClauseEnum::PATTERN;
      std::string clause_with_pattern_removed = string_util::RemoveFirstWord(curr_clause);
      std::vector<QueryToken> tokens = processPatternClause(clause_with_pattern_removed, declarations);
      for (const QueryToken& token : tokens) {
        pattern_tokens.push_back(token);
      }
    } else if (clauseMatch(curr_clause, qps_constants::kWithClauseRegex)) {
      prev_clause = ClauseEnum::WITH;
      std::string clause_with_with_removed = string_util::RemoveFirstWord(curr_clause);
      std::vector<QueryToken> tokens = processWithClause(clause_with_with_removed, declarations);
      for (const QueryToken& token : tokens) {
        with_tokens.push_back(token);
      }
    } else {
      throw QpsSyntaxError("Unrecognized clause");
    }
  }
  return {such_that_tokens, pattern_tokens, with_tokens};
}

std::vector<QpsSemanticError> QueryTokenizer::semantic_errors;

void QueryTokenizer::checkSemanticErrors() {
  if (!semantic_errors.empty()) {
    QpsSemanticError first_error = semantic_errors.at(0);
    throw QpsSemanticError(first_error.what());
  }
}

TokenisedQuery QueryTokenizer::tokenize(const std::string& query) {
  semantic_errors = {};
  std::vector<QueryToken> tokens;
  std::string sanitized_query = string_util::RemoveWhiteSpaces(query);
  QueryStructure statements = QueryTokenizer::splitQuery(sanitized_query);

  std::vector<Declaration>
      declarations = QueryTokenizer::extractDeclarations(statements.declaration_statements);
  std::vector<QueryToken>
      select_tokens = QueryTokenizer::extractSelectToken(statements.select_statement, declarations);
  std::vector<std::vector<QueryToken>>
      clause_tokens = QueryTokenizer::extractClauseTokens(statements.select_statement, declarations);
  QueryTokenizer::checkSemanticErrors();

  std::vector<QueryToken> such_that_tokens = clause_tokens[0];
  std::vector<QueryToken> pattern_tokens = clause_tokens[1];
  std::vector<QueryToken> with_tokens = clause_tokens[2];

  return {
      declarations,
      select_tokens,
      such_that_tokens,
      pattern_tokens,
      with_tokens
  };
}
