#pragma once

#include <string>
#include <vector>
#include <utility>
#include <regex>
#include "qps/declaration.h"

// Token types
enum class PQLTokenType {
  SYNONYM,
  DECLARATION,
  SELECT,
  SUCH_THAT,
  PATTERN_WHILE,
  PATTERN_IF,
  WILDCARD,
  RELREF,
  INTEGER,
  IDENT,
  PARTIALEXPR,
  EXACTEXPR,
  SELECT_SINGLE,
  SELECT_MULTIPLE,
  SELECT_BOOLEAN,
  WITH_PROCNAME,
  WITH_VARNAME,
  WITH_VALUE,
  WITH_STMTNO
};

struct QueryToken {
  std::string text;
  PQLTokenType type;
};

struct TokenisedQuery {
  std::vector<Declaration> declarations;
  std::vector<QueryToken> select_tokens;
  std::vector<QueryToken> such_that_tokens;
  std::vector<QueryToken> pattern_tokens;
  std::vector<QueryToken> with_tokens;
};

struct QueryStructure {
  QueryStructure(std::vector<std::string> declaration_statements, std::string select_statement);
  std::vector<std::string> declaration_statements;
  std::string select_statement;
};

enum class ClauseEnum {
  PATTERN,
  SUCH_THAT,
  WITH,
  NONE
};

enum class SelectValueType {
  SINGLE,
  MUTLIPLE,
};

class QueryTokenizer {
 public:
  /*!
   * Tokenizes the query
   * @param query the string query
   * @return a TokenisedQuery
   */
  static TokenisedQuery tokenize(const std::string & query);

  /*!
   * Splits sanitized query into declarations and select statements
   * @param sanitized_query query without unnecessary white spaces
   * @return QueryStructure
   */
  static QueryStructure splitQuery(std::string sanitized_query);

  /*!
   * Extracts the a vector of declarations from a vector of strings
   * @param declaration_statements vector of strings to be extracted
   * @return vector of declarations
   */
  static std::vector<Declaration> extractDeclarations(const std::vector<std::string> & declaration_statements);

  /*!
   * Extracts a vector of QueryTokens from select statement
   * @param select_statement
   * @param declarations vector of declarations
   * @return a vector of QueryTokens relevant to Select clause
   */
  static std::vector<QueryToken> extractSelectToken(std::string & select_statement,
                                                    std::vector<Declaration> & declarations);

  /*!
 * Returns the starting index of the first clause
 * @param remaining_statement is the trimmed statement from select statement
 * @return starting index for the first clause
 */
  static size_t getFirstClauseIndexes(const std::string & remaining_statement);

  /*!
   * Returns the starting indexes of clauses
   * @param remaining_statement is the trimmed statement from select statement
   * @return a vector of starting indexes for such that and pattern clauses
   */
  static std::vector<size_t> getClauseIndexes(const std::string & remaining_statement);

  /*!
   * Checks if the clause is able to be a such that clause or pattern clause
   * @param clause is the clause to check
   * @param regexPattern a regex pattern specific to such that or pattern clause
   * @return true if clause matches a such that clause or pattern clause, else false
   */
  static bool clauseMatch(std::string & clause, const std::regex & regexPattern);
  /*!
   * Returns the LHS and RHS of relationship reference query as a pair
   * @param clause is the trimmed string from a relationship reference
   * @param declarations is the set of declared entities
   * @return the LHS and RHS as a pair
   */
  static std::pair<QueryToken, QueryToken> getRelRefArgs(std::string & clause,
                                                         std::vector<Declaration> & declarations);

  /*!
   * Returns the LHS and RHS of pattern clause
   * @param clause is the trimmed string from pattern clause
   * @param declarations is the set of declared entities
   * @param pattern_type
   * @return the LHS and RHS as a pair
   */
  static std::pair<QueryToken, QueryToken> getPatternArgs(std::string & clause,
                                                          std::vector<Declaration> & declarations,
                                                          PQLTokenType pattern_type);

  /*!
   * Returns the LHS and RHS of with clause
   * @param clause is the trimmed string from with clause
   * @param declarations is the set of declared entities
   * @return the LHS and RHS as a pair
   */
  static std::pair<QueryToken, QueryToken> getWithArgs(std::string & clause,
                                                       std::vector<Declaration> & declarations);

  /*!
   * Gets the PQLTokenType of a pattern
   * @param pattern_syn to be checked
   * @return SYNONYM if it is an assign synonym, PATTERN_WHILE if it is 'while', PATTERN_IF if it is 'if'
   */
  static PQLTokenType getPatternTokenType(std::string & pattern_syn, std::vector<Declaration> & declarations);

  /*!
   * Returns the query tokens of such that and pattern clauses
   * @param select_statement is the second statement of a query
   * @param declarations is the set of declared entities
   * @return a pair of vectors of query tokens
   */
  static std::vector<std::vector<QueryToken>> extractClauseTokens(std::string select_statement,
                                                                  std::vector<Declaration> & declarations);

  /*!
   * Processes a such that clause
   * @param clause string to be processed
   * @param declarations set of declared entities
   * @return a vector of such that tokens
   */
  static std::vector<QueryToken> processSuchThatClause(std::string clause, std::vector<Declaration> & declarations);

  /*!
   * Processes a pattern clause
   * @param clause string to be processed
   * @param declarations set of declared entities
   * @return a vector of pattern tokens
   */
  static std::vector<QueryToken> processPatternClause(std::string clause, std::vector<Declaration> & declarations);

  /*!
   * Processes a with clause
   * @param clause string to be processed
   * @param declarations set of declared entities
   * @return a vector of with tokens
   */
  static std::vector<QueryToken> processWithClause(std::string clause, std::vector<Declaration> & declarations);

  /*!
   * Gets the select value type from a select clause
   * @param select_value is the string to be processed
   * @return the select value type
   */
  static SelectValueType getSelectValueType(const std::string& select_value);

  /*!
   * Removes the select clause from remaining clauses
   * @param remaining_statement is string to be processed
   * @return the trimmed string
   */
  static std::string removeSelectClause(const std::string& remaining_statement);

  /*!
   * Removes the tuple or first word from a select clause
   * @param select_statement to be processed
   * @return the trimmed select clause
   */
  static std::string removeResultClause(std::string& select_statement);

  /*!
   * Preserves the result clause of the select clause
   * @param select_statement to be processed
   * @return the preserved result clause
   */
  static std::string removeAfterResultClause(std::string& select_statement);
};
