#pragma once

// Headers from standard libraries
#include <vector>
#include <memory>
#include <string>

// Headers from "SP" subdirectory
#include "SP/TNode.h"
#include "SP/sp_tokeniser/Token.h"
#include "SP/sp_tokeniser/TokenTypes.h"
#include "SP/DesignExtractor.h"
#include "SP/sp_parser/Parser.h"
#include "SP/utils/ParseUtils.h"

// Headers from other directories
#include "PKB/PKB.h"
#include "utils/Error.h"

class ParseUtils {
 public:
  static bool isPlusOrMinus(const Token& token);
  static bool isMultiplyDivideOrModulo(const Token& token);
  static bool isVarOrConst(const Token& token);
  static bool isCondExpressionOperator(const Token& token);
  static bool isRelFactorOperator(const Token& token);
  static TokenType convertLiteralToEntity(std::string value);

  static int index;
  static int lineNumber;
  static void incrementIndex();
  static int getIndex();
  static void setValues(int index, int lineNumber);
  static std::shared_ptr<TNode> parseExpression(const std::vector<Token>& tokens);
  static std::shared_ptr<TNode> parseTerm(const std::vector<Token>& tokens);
  static std::shared_ptr<TNode> parseFactor(const std::vector<Token>& tokens);
  static std::shared_ptr<TNode> parseCondExpression(const std::vector<Token>& tokens);
  static std::shared_ptr<TNode> parseRelExpression(const std::vector<Token>& tokens);
  static std::shared_ptr<TNode> parseRelFactor(const std::vector<Token>& tokens);
};
