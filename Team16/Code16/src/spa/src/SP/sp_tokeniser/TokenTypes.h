#pragma once

/**
 * @brief Enumeration representing different token types used in the Simple Programming Language (SPL).
 *
 * The `TokenType` enum defines various token types that are used to classify and categorize elements
 * in the Simple Programming Language (SPL). These token types include keywords, literals, operators,
 * separators, and an unknown error type for unmatched tokens.
 */
enum class TokenType {
  // Keywords
  kEntityIf,
  kEntityElse,
  kEntityThen,
  kEntityWhile,
  kEntityRead,
  kEntityProcedure,
  kEntityStmt,
  kEntityStmtLst,
  kEntityPrint,
  kEntityAssign,
  kEntityCall,
  kEntityVariable,
  kEntityConstant,

  // Literals
  kLiteralLetter,
  kLiteralDigit,
  kLiteralInteger,
  kLiteralName,

  // Operators
  kOperatorPlus,
  kOperatorMinus,
  kOperatorMultiply,
  kOperatorDivide,
  kOperatorEqual,
  kOperatorMod,
  kOperatorNotEqual,
  kOperatorLess,
  kOperatorLessEqual,
  kOperatorGreater,
  kOperatorGreaterEqual,
  kOperatorLogicalAnd,
  kOperatorLogicalOr,
  kOperatorLogicalNot,

  // Separators
  kSepComma,
  kSepSemicolon,
  kSepOpenParen,
  kSepCloseParen,
  kSepOpenBrace,
  kSepCloseBrace,

  // Error
  kUnknownTokenType,

  // Others
  kWhiteSpace,
};
