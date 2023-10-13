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
#include <map>

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
    /**
      * @brief Checks the brace stack and adds tokens.
      *
      * This method checks the brace stack to ensure balanced parentheses and braces and adds the corresponding tokens
      * to the provided vector of tokens.
      *
      * @param braceStack A stack of characters representing open braces and parentheses.
      * @param tokens A vector of tokens where tokens are added if balanced.
      * @param matchedType The token type of the current token.
      * @param matchedValue The matched value of the current token.
    */
    void checkBraceStack(std::stack<char>& braceStack, std::vector<Token>& tokens, TokenType matchedType, std::string matchedValue); 
    /**
     * @brief Matches a regular expression and returns the token type and value.
     *
     * This method matches a regular expression against a given line of code and returns a pair containing the token type
     * and the matched value.
     *
     * @param line The line of code to match against.
     * @return A pair containing the token type and the matched value.
     */
    std::pair<TokenType, std::string> matchRegex(const std::string& line);
    /**
      * @brief Checks the validity of a literal and adds it to tokens.
      *
      * This method checks if a matched literal is valid and adds it to the provided vector of tokens. It ensures that
      * literals do not start with a digit, indicating a valid name.
      *
      * @param tokens A vector of tokens where valid literals are added.
      * @param matchedType The token type of the current token.
      * @param matchedValue The matched value of the current token.
     */
    void checkValidLiteral(std::vector<Token>& tokens, TokenType matchedType, std::string matchedValue);
};
