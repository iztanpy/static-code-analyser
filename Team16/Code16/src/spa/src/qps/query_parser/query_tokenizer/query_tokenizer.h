#pragma once

#include <string>
#include <vector>
#include <utility>
#include "qps/declaration.h"

// Token types
enum class PQLTokenType {
  SYNONYM,
  DECLARATION,
  SELECT,
  SUCH_THAT,
  PATTERN,
  WILDCARD,
  RELREF,
  INTEGER,
  IDENT,
  PARTIALEXPR
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
};

struct QueryStructure {
  QueryStructure(std::vector<std::string> declaration_statements, std::string select_statement);
  std::vector<std::string> declaration_statements;
  std::string select_statement;
};

class QueryTokenizer {
 public:
  static TokenisedQuery tokenize(const std::string & query);
  static bool isEnclosedInDoubleQuotes(const std::string & token);
  static bool isEnclosedInWildcards(const std::string & token);

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
                                                    const std::vector<Declaration> & declarations);

  /*!
   * Returns the starting indexes of clauses
   * @param remaining_statement is the trimmed statement from select statement
   * @return a vector of starting indexes for such that and pattern clauses
   */
  static std::vector<size_t> getClauseIndexes(const std::string & remaining_statement);

  /*!
   * Returns the LHS and RHS of relationship reference query as a pair
   * @param clause is the trimmed string from a relationship reference
   * @param declarations is the set of declared entities
   * @return the LHS and RHS as a pair
   */
  static std::pair<QueryToken, QueryToken> getRelRefArgs(std::string & clause,
                                                         const std::vector<Declaration> & declarations);

  /*!
   * Returns the LHS and RHS of pattern clause
   * @param clause is the trimmed string from pattern clause
   * @param declarations is the set of declared entities
   * @return the LHS and RHS as a pair
   */
  static std::pair<QueryToken, QueryToken> getPatternArgs(std::string & clause,
                                                          const std::vector<Declaration> & declarations);

  /*!
   * Returns the query tokens of such that and pattern clauses
   * @param select_statement is the second statement of a query
   * @param declarations is the set of declared entities
   * @return a pair of vectors of query tokens
   */
  static std::pair<std::vector<QueryToken>, std::vector<QueryToken>>
  extractClauseTokens(std::string select_statement,
                      const std::vector<Declaration> & declarations);
};
