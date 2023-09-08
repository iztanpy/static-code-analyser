#ifndef TOKEN_H
#define TOKEN_H
#include "TokenTypes.h"

struct Token {
    TokenType tokenType;
    std::string value;
    int lineNumber;
    int linePosition;
};

#endif