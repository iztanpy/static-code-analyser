#include "ParserFactory.h"

std::pair<int, std::string> ParserFactory::parseStatements(std::vector<Token> tokens, int curr_index) {
    while (curr_index < tokens.size()) {
        Token curr_token = tokens[curr_index];
        if (curr_token.tokenType == TokenType::kLiteralName) {
            Token next_token = tokens.at(curr_index + 1);

            if (next_token.tokenType == TokenType::kEntityAssign) {
                assignmentParser->lineNumber = lineNumber;
                int next_index = assignmentParser->parse(tokens, curr_index);

                if (next_index == -1) {
                  throw std::runtime_error("Syntactic error! We don't support anything and everything.");
                } else {
                    lineNumber++;
                    curr_index = next_index;
                    return std::make_pair(curr_index, "Read");
                }
            }
        } else if (curr_token.tokenType == TokenType::kEntityRead) {
            readParser->lineNumber = lineNumber;
            int next_index = readParser->parse(tokens, curr_index);

            if (next_index == -1) {
              throw std::runtime_error("Syntactic error! We don't support anything and everything.");
            } else {
                lineNumber++;
                curr_index = next_index;
                return std::make_pair(curr_index, "Read");
            }
        } else {
            throw std::runtime_error("Syntactic error! We don't support anything and everything.");
        }
    }
}

