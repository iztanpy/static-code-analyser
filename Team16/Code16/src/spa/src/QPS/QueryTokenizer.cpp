// ai-gen start (gpt3, 2)
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include "QueryTokenizer.h"

std::vector<QueryToken> QueryTokenizer::tokenize(const std::string &query) {
    std::vector<QueryToken> tokens;
    std::string sanitizedQuery;

    // Replace multiple consecutive whitespaces and new lines with a single space
    bool inWhitespace = false;
    std::set<char> queryDelimiters{',', ';'};

    for (char c : query) {
        if (std::isspace(c) || queryDelimiters.find(c) != queryDelimiters.end()) {
            if (!inWhitespace) {
                sanitizedQuery += ' ';
                inWhitespace = true;
            }
        } else {
            sanitizedQuery += c;
            inWhitespace = false;
        }
    }

    std::istringstream tokenizer(sanitizedQuery);
    std::string token;
    std::set<std::string> designEntities{"stmt", "read", "print", "while", "if", "assign",
                                         "variable", "constant", "procedure"};

    while (std::getline(tokenizer, token, ' ')) {
        if (!token.empty()) {
            // Determine the token type
            TokenType type;
            // if token contains a design entity ==> it is a declaration
            if (designEntities.find(token) != designEntities.end()) {
                type = TokenType::DECLARATION;
            } else if (token == "Select") {
                type = TokenType::SELECT;
            } else if (token == "such" || token == "that") {
                type = TokenType::SUCH_THAT;
                /*
                 * TODO: must handle this case during validation
                 * - a TokenType::SUCH_THAT comes in a PAIR of tokens
                 * - tokens = [...,{text: 'such', TokenType::SUCH_THAT}, {text: 'that', TokenType::SUCH_THAT},...]
                 * - otherwise the query is invalid
                 */
            } else if (token == "pattern") {
                type = TokenType::PATTERN;
            } else {
                type = TokenType::SYNONYM;
            }

            // Store the token and its type
            tokens.push_back({token, type});
        }
    }

    return tokens;
}

// Helper function to split a string based on a delimiter
std::vector<std::string> QueryTokenizer::splitString(const std::string &str, char delimiter) {
    std::vector<std::string> parts;
    std::istringstream stream(str);
    std::string part;
    while (std::getline(stream, part, delimiter)) {
        parts.push_back(part);
    }
    return parts;
}
// ai-gen end
