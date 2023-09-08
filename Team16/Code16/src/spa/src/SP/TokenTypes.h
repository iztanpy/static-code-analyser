#ifndef TOKENTYPES_H
#define TOKENTYPES_H

enum TokenType {
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


#endif