/* Created by Tiffany, Shaine */
// ai-gen start (gpt3, 2)

#pragma once

#include <string>
#include <utility>
#include "SP/sp_tokeniser/token_types.h"

/**
 * @struct Token
 *
 * Represents a token in the program.
 */
struct Token {
  TokenType tokenType;
  std::string value;

  /**
   * Constructor with a specified token type.
   */
  Token() : tokenType(), value() {}

  /**
   * Constructor with a token type and line number.
   * @param t The type of the token.
   */
  explicit Token(TokenType t) : tokenType(t), value() {}

  /**
  * Constructor with a token type, value, and line number.
  * @param t The type of the token.
  * @param v The value of the token.
  */
  Token(TokenType t, std::string v) : tokenType(t), value(std::move(v)) {}

  [[nodiscard]] std::string getValue() const {
    return value;
  }
};
