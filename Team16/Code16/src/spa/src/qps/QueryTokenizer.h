#pragma once

#include <stdio.h>
#include <string>
#include <vector>

// Token types
enum class TokenType {
    SYNONYM,
    DECLARATION,
    SELECT,
    SUCH_THAT,
    PATTERN
};

struct QueryToken {
    std::string text;
    TokenType type;
};

class QueryTokenizer {
 public:
    static std::vector<QueryToken> tokenize(const std::string& query);

    // Helper function to split a string based on a delimiter
    static std::vector<std::string> splitString(const std::string& str, char delimiter);
};
