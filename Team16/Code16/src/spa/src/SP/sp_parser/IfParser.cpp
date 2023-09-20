#include "IfParser.h"

int IfParser::parse(const std::vector<Token>& tokens, int curr_index) {
    std::vector<Token> conditionTokens = IfParser::getConditionTokens(tokens, curr_index);
    int validCondition = conditionParser->parse(conditionTokens, 0);
    if (validCondition == -1) {
        throw InvalidSyntaxError();
    }

    // verify then keyword
    if (!(curr_index + conditionTokens.size() + 1 < tokens.size())) {
        throw InvalidSyntaxError();
    }
    Token thenToken = tokens[curr_index + conditionTokens.size() + 1];
    if (thenToken.tokenType != TokenType::kEntityThen) throw InvalidSyntaxError();

    return curr_index + conditionTokens.size() + 3;  // Continue evaluation and skip pass closing brace
}

std::vector<Token> IfParser::getConditionTokens(const std::vector<Token>& tokens, int curr_index) {
    // Initialize condition tokens
    std::vector<Token> conditionTokens;

    // curr_index points to the "while" keyword
    if (curr_index + 1 < tokens.size() && tokens[curr_index + 1].tokenType == TokenType::kSepOpenParen) {
        int openParenCount = 1;
        conditionTokens.push_back(tokens[curr_index + 1]);
        int closeParenIndex = curr_index + 2;
        while (closeParenIndex < tokens.size() && openParenCount > 0) {
            if (tokens[closeParenIndex].tokenType == TokenType::kSepOpenParen) {
                openParenCount++;
            } else if (tokens[closeParenIndex].tokenType == TokenType::kSepCloseParen) {
                openParenCount--;
            }
            conditionTokens.push_back(tokens[closeParenIndex]);  // Add tokens within the condition
            closeParenIndex++;
        }

        if (openParenCount > 0) {
            // Implies that there is a missing closing parenthesis somewhere
            throw InvalidSyntaxError();
        }
        curr_index = closeParenIndex;  // Modify curr_index to the new position
    } else {
        // Implies that there is a missing open parenthesis somewhere
        throw InvalidSyntaxError();
    }
    return conditionTokens;
}
