#pragma once

#include "SP/sp_tokeniser/Token.h"

class ParseUtils {
 public:
  static bool isPlusOrMinus(const Token& token);
  static bool isMultiplyDivideOrModulo(const Token& token);
  static bool isVarOrConst(const Token& token);
};
