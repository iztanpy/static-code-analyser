// ai-gen start (gpt3, 2)
#include "WhileParser.h"

int WhileParser::parse(const std::vector<Token>& tokens, int curr_index) {
    // Validate that statement has at least 5 tokens (min: lhs = rhs ;)
    if (tokens.size() - index < 4) {
      return -1;
    }
    index++;

    // Validate open parenthesis
    if (tokens[index].tokenType != TokenType::kSepOpenParen) {
        throw InvalidSyntaxError();
    }
    index++;

    ParseUtils::setValues(index, lineNumber);
    std::shared_ptr<TNode> whileCondNode = ParseUtils::parseCondExpression(tokens);
    index = ParseUtils::getIndex();

    // Validate close parenthesis
    if (tokens[index].tokenType != TokenType::kSepCloseParen) {
      throw InvalidSyntaxError();
    }
    index++;

    // Validate open braces
    if (tokens[index].tokenType != TokenType::kSepOpenBrace) {
      throw InvalidSyntaxError();
    }
    index++;

    designExtractor->extractDesign(whileCondNode, visitor);

    return index;
}

std::vector<Token> WhileParser::getConditionTokens(const std::vector<Token>& tokens, int curr_index) {
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
