// ai-gen start (gpt3, 2)
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include "qps/query_tokenizer.h"
#include "qps/design_entity.h"

std::vector<QueryToken> QueryTokenizer::tokenize(const std::string& query) {
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
    std::set<std::string> stringDesignEntities = Entity::getStringDesignEntities();

    while (std::getline(tokenizer, token, ' ')) {
        if (!token.empty()) {
            // Determine the token type
            PQLTokenType type;
            // if token contains a design entity ==> it is a declaration
            if (stringDesignEntities.find(token) != stringDesignEntities.end()) {
                type = PQLTokenType::DECLARATION;
            } else if (token == "Select") {
                type = PQLTokenType::SELECT;
            } else if (token == "such" || token == "that") {
                type = PQLTokenType::SUCH_THAT;
                /*
                 * TODO: must handle this case during validation
                 * - a PQLTokenType::SUCH_THAT comes in a PAIR of tokens
                 * - tokens = [...,{text: 'such', PQLTokenType::SUCH_THAT}, {text: 'that', PQLTokenType::SUCH_THAT},...]
                 * - otherwise the query is invalid
                 */
            } else if (token == "pattern") {
                type = PQLTokenType::PATTERN;
            } else {
                type = PQLTokenType::SYNONYM;
            }

            // Store the token and its type
            tokens.push_back({token, type});
        }
    }

    return tokens;
}
// ai-gen end
