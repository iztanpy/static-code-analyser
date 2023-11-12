#pragma once

// Headers from standard libraries
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <stack>

// Headers from "SP" subdirectory
#include "SP/tnode.h"
#include "SP/sp_tokeniser/token.h"
#include "SP/sp_tokeniser/token_types.h"
#include "SP/design_extractor.h"
#include "SP/sp_parser/parser.h"

// Headers from other directories
#include "PKB/pkb.h"
#include "utils/error.h"

typedef std::unordered_map<std::string, TokenType> EntityMap;

/**
 * @class parse_utils
 * @brief A utility class containing static methods for parsing tokens.
 *
 * The `parse_utils` class provides a collection of static methods for parsing and analyzing tokens
 * used in SIMPLE. These methods assist in parsing expressions, factors,
 * and conditional expressions, as well as converting literals to entity types.
 */
class parse_utils {
 private:
  static int index;
  static int lineNumber;
  inline static std::string procedureName;
  static std::unordered_map<int, std::vector<int>> condIndexMap;
  static EntityMap entityMap;
  /**
   * @brief Sets up an internal map of keywords to TokenTypes.
   */
  static void setUpEntityMap();
  /**
   * @brief Checks if the token represents an addition or subtraction operator.
   * @param token The token to be checked.
   * @return True if the token is a plus or minus operator, otherwise false.
   */
  static bool isPlusOrMinus(const Token& token);
  /**
   * @brief Checks if the token represents a multiplication, division, or modulo operator.
   * @param token The token to be checked.
   * @return True if the token is a multiply, divide, or modulo operator, otherwise false.
   */
  static bool isMultiplyDivideOrModulo(const Token& token);
  /**
   * @brief Checks if the token represents a variable or constant.
   * @param token The token to be checked.
   * @return True if the token is a variable or constant, otherwise false.
   */
  static bool isVarOrConst(const Token& token);
  /**
   * @brief Checks if the token represents a conditional expression operator.
   * @param token The token to be checked.
   * @return True if the token is a conditional expression operator, otherwise false.
   */
  static bool isCondExpressionOperator(const Token& token);
  /**
   * @brief Checks if the token represents a relational factor operator.
   * @param token The token to be checked.
   * @return True if the token is a relational factor operator, otherwise false.
   */
  static bool isRelFactorOperator(const Token& token);
  /**
   * @brief Parses a term from a vector of tokens.
   * @param tokens The vector of tokens representing the term.
   * @return A shared pointer to the parsed term node.
   */
  static std::shared_ptr<TNode> parseTerm(const std::vector<Token>& tokens);
  /**
   * @brief Parses a factor from a vector of tokens.
   * @param tokens The vector of tokens representing the factor.
   * @return A shared pointer to the parsed factor node.
   */
  static std::shared_ptr<TNode> parseFactor(const std::vector<Token>& tokens);
  /**
   * @brief Parses a relational expression from a vector of tokens.
   * @param tokens The vector of tokens representing the relational expression.
   * @return A shared pointer to the parsed relational expression node.
   */
  static std::shared_ptr<TNode> parseRelExpression(const std::vector<Token>& tokens);
  /**
   * @brief Parses a relational factor from a vector of tokens.
   * @param tokens The vector of tokens representing the relational factor.
   * @return A shared pointer to the parsed relational factor node.
   */
  static std::shared_ptr<TNode> parseRelFactor(const std::vector<Token>& tokens);

 public:
  /**
   * @brief Converts a literal entity to the corresponding entity token type.
   * @param value The literal entity to be converted.
   * @return The entity token type corresponding to the literal entity.
   */
  static TokenType convertLiteralToEntity(const std::string& value);
  /**
   * @brief Set Procedure Name of the current procedure.
   * @param procedureName Procedure Name of current procedure.
   */
  static void setProcedureName(std::string procedureName);
  /**
   * @brief Get the name of the current procedure.
   * @return Procedure Name of current procedure.
   */
  static std::string getProcedureName();
  /**
   * @brief Increments the static index value.
   */
  static void incrementIndex();
  /**
   * @brief Gets the current static index value.
   * @return The current index value.
   */
  static int getIndex();
  /**
   * @brief Sets the static index and line number values.
   * @param index The index value to set.
   * @param lineNumber The line number value to set.
   */
  static void setValues(int index, int lineNumber);
  /**
   * @brief Parses an expression from a vector of tokens.
   * @param tokens The vector of tokens representing the expression.
   * @return A shared pointer to the parsed expression node.
   */
  static std::shared_ptr<TNode> parseExpression(const std::vector<Token>& tokens);
  /**
   * @brief Parses a conditional expression from a vector of tokens.
   * @param tokens The vector of tokens representing the conditional expression.
   * @return A shared pointer to the parsed conditional expression node.
   */
  static std::shared_ptr<TNode> parseCondExpression(const std::vector<Token>& tokens);
  /**
   * @brief Sets up a map of start, middle and end index of conditional expressions.
   * @param tokens The vector of tokens representing the relational factor.
   */
  static void setUpCondIndexMap(const std::vector<Token>& tokens);
};
