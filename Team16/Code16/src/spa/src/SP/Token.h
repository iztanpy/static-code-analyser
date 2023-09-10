#ifndef SPA_TOKEN_H
#define SPA_TOKEN_H
#include <string>
#include <utility>
#include "TokenTypes.h"

struct Token {
    TokenType tokenType;
    std::string value;
    int lineNumber;
    int linePosition;

    Token() : tokenType(), lineNumber(), value(), linePosition() {};
    Token(TokenType t) : tokenType(t), lineNumber(), value(), linePosition() {};
    Token(TokenType t, int l) : tokenType(t), lineNumber(l), value(), linePosition() {};
    Token(TokenType t, std::string v, int l) : tokenType(t), lineNumber(l), value(std::move(v)), linePosition() {};
    Token(TokenType t, std::string v, int l, int lp) : tokenType(t), lineNumber(l), value(std::move(v)), linePosition(lp) {};

    [[nodiscard]] std::string getValue() const {
        return value;
    }
};

struct SPToken {
    TokenType tokenType;
    std::string value;
    int lineNumber;
    int linePosition;

    SPToken() : tokenType(), value(), lineNumber(), linePosition() {};

    SPToken(TokenType t, std::string v, int lineNum, int linePos) : tokenType(t), value(std::move(v)), lineNumber(lineNum),
                                                                  linePosition(linePos) {};
};

#endif //SPA_TOKEN_H
