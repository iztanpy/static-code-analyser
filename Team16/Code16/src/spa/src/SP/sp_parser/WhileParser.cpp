// ai-gen GPT-3.5 

#include "WhileParser.h"

int WhileParser::parse(const std::vector<Token>& tokens, int curr_index) {
    std::vector<Token> conditionTokens = WhileParser::getConditionTokens(tokens, curr_index); 
    while (curr_index < tokens.size()) {
        // validate the close brackets of the while loop 
        Token currToken = tokens[curr_index]; 
        if (currToken.tokenType != TokenType::kSepOpenBrace) { // invalid loop - does not contain open brace
            throw InvalidSyntaxError(); 
        }
        // now we need to extract the statements within the loop 
        try {
            std::pair<int, std::string> results = factory.parseStatements(tokens, curr_index);
        }
        catch (const InvalidSyntaxError& e) {
            e.log(); 
        }
        // after extracting the statements, we validate that the close brace still exists
    }
    return curr_index;
}

std::vector<Token> WhileParser::getConditionTokens(const std::vector<Token>& tokens, int& curr_index) {
    // initialize condition tokens 
    std::vector<Token> conditionTokens;

    // curr_index points to the while keyword 
    if (curr_index + 1 < tokens.size() && tokens[curr_index + 1].tokenType == TokenType::kSepOpenParen) {
        int openParenCount = 1;
        int closeParenIndex = curr_index + 2;

        while (closeParenIndex < tokens.size() && openParenCount > 0) {
            if (tokens[closeParenIndex].tokenType == TokenType::kSepOpenParen) {
                openParenCount++;
            }
            else if (tokens[closeParenIndex].tokenType == TokenType::kSepCloseParen) {
                openParenCount--;
            }
            if (openParenCount > 0) {
                conditionTokens.push_back(tokens[closeParenIndex]); // Add tokens within the condition
            }
            closeParenIndex++;
        }

        if (openParenCount > 0) {
            // implies that there is a missing closing parenthesis somewhere
            throw InvalidSyntaxError();
        }
        curr_index = closeParenIndex; // modify curr_index to the new position
    } else {
        // implies that there is a missing open parenthesis somewhere 
        throw InvalidSyntaxError();
    }
    return conditionTokens; 
}





