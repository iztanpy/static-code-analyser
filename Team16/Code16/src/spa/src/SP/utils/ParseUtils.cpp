#include "ParseUtils.h"

bool ParseUtils::isVarOrConst(const Token& token) {
  return token.tokenType == TokenType::kLiteralName ||
      token.tokenType == TokenType::kLiteralInteger;
}

bool ParseUtils::isPlusOrMinus(const Token& token) {
  return token.tokenType == TokenType::kOperatorPlus ||
      token.tokenType == TokenType::kOperatorMinus;
}

bool ParseUtils::isMultiplyDivideOrModulo(const Token& token) {
  return token.tokenType == TokenType::kOperatorMultiply ||
      token.tokenType == TokenType::kOperatorDivide ||
      token.tokenType == TokenType::kOperatorMod;
}
