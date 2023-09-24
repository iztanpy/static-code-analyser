#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "TokenTypes.h"
#include "Token.h"
#include "utils/Error.h"
#include <regex>
#include <set>
#include <utility>
#include <stack>
#include <unordered_set>

/**
 * @class SPtokeniser
 * @brief A class for tokenizing input code into a vector of tokens.
 *
 * The `SPtokeniser` class provides methods for tokenizing input code into a vector of tokens.
 * It performs lexical analysis to recognize and classify different elements of the code.
 */
class SPtokeniser {
 public:
    /**
     * @class SPtokeniser
     * @brief A class for tokenizing input code into a vector of tokens.
     *
     * The `SPtokeniser` class provides methods for tokenizing input code into a vector of tokens.
     * It performs lexical analysis to recognize and classify different elements of the code.
    */
    std::vector<std::string> splitLines(const std::string& input);
    /**
     * @brief Tokenizes the input code and returns a vector of tokens.
     *
     * This method takes an input code as a string and tokenizes it into individual tokens.
     * It returns a vector of `Token` structures, each containing token type, lexeme, and line number.
     *
     * @param input The input code to be tokenized.
     * @return A vector of `Token` structures representing individual tokens.
     */
    std::vector<struct Token> tokenise(const std::string& input);
};
