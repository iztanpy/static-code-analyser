/* Created by Tiffany, Shaine */
// ai-gen start (gpt3, 2)


#ifndef TEAM16_CODE16_SRC_SPA_SRC_SP_TOKEN_H_
#define TEAM16_CODE16_SRC_SPA_SRC_SP_TOKEN_H_

#include <string>
#include <utility>
#include "TokenTypes.h"

/**
 * @struct Token
 *
 * Represents a token in the program.
 */
struct Token {
    TokenType tokenType;
    std::string value;
    int lineNumber;
    int linePosition;

    /**
     * Constructor with a specified token type.
     * @param t The type of the token.
     */
    Token() : tokenType(), value(), lineNumber(), linePosition() {}

    /**
     * Constructor with a token type and line number.
     * @param t The type of the token.
     * @param l The line number where the token is found.
     */
    explicit Token(TokenType t) : tokenType(t), lineNumber(), value(), linePosition() {}

    /**
     * Constructor with a token type, value, and line number.
     * @param t The type of the token.
     * @param l The line number where the token is found.
     */
    Token(TokenType t, int l) : tokenType(t), lineNumber(l), value(), linePosition() {}

    /**
    * Constructor with a token type, value, and line number.
    * @param t The type of the token.
    * @param v The value of the token.
    * @param l The line number where the token is found.
    */
    Token(TokenType t, std::string v, int l) : tokenType(t), lineNumber(l), value(std::move(v)), linePosition() {}

    /**
    * Constructor with a token type, value, line number, and line position.
    * @param t The type of the token.
    * @param v The value of the token.
    * @param l The line number where the token is found.
    * @param lp The position within the line where the token starts.
    */
    Token(TokenType t, std::string v, int l, int lp) :
        tokenType(t), value(std::move(v)), lineNumber(l), linePosition(lp) {}

    [[nodiscard]] std::string getValue() const {
        return value;
    }
};


/**
 * @struct SPToken
 *
 * Represents a specialized token in the program.
 */
struct SPToken {
    TokenType tokenType;
    std::string value;
    int lineNumber;
    int linePosition;

    SPToken() : tokenType(), value(), lineNumber(), linePosition() {}

    /**
     * Constructor with a token type, value, line number, and line position.
     * @param t The type of the token.
     * @param v The value of the token.
     * @param lineNum The line number where the token is found.
     * @param linePos The position within the line where the token starts.
     */
    SPToken(TokenType t, std::string v, int lineNum, int linePos) :
        tokenType(t), value(std::move(v)), lineNumber(lineNum),
        linePosition(linePos) {}
};

#endif  // TEAM16_CODE16_SRC_SPA_SRC_SP_TOKEN_H_
