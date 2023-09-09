#ifndef TEAM16_CODE16_SRC_SPA_SRC_SP_TOKEN_H_
#define TEAM16_CODE16_SRC_SPA_SRC_SP_TOKEN_H_
#include "TokenTypes.h"
#include <string>

struct Token {
    TokenType tokenType;
    std::string value;
    int lineNumber;
    int linePosition;
};

#endif  // TEAM16_CODE16_SRC_SPA_SRC_SP_TOKEN_H_
