#ifndef TEAM16_CODE16_SRC_SPA_SRC_SP_TOKENTYPES_H_
#define TEAM16_CODE16_SRC_SPA_SRC_SP_TOKENTYPES_H_

enum class TokenType {
    // Keywords
    kEntityIf,
    kEntityElse,
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

#endif  // TEAM16_CODE16_SRC_SPA_SRC_SP_TOKENTYPES_H_
