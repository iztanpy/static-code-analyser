#include <iostream>
#include <vector>
#include "SimpleParser.h"

SimpleParser::SimpleParser(WriteFacade* writeFacadePtr, ASTVisitor* astVisitorPtr) : writeFacade(writeFacadePtr),
visitor(astVisitorPtr) {}

int SimpleParser::parse(const std::vector<Token>& tokens, int curr_index) {
    while (curr_index < tokens.size()) {
        Token curr_token = tokens[curr_index];

        if (curr_token.tokenType == TokenType::kLiteralName) {
            Token next_token = tokens.at(curr_index + 1);

            if (next_token.tokenType == TokenType::kEntityAssign) {
                assignmentParser->lineNumber = lineNumber;
                int next_index = assignmentParser->parse(tokens, curr_index);

                if (next_index == -1) {
                    throw InvalidSyntaxError();
                } else {
                    lineNumber++;
                    curr_index = next_index;
                }
            }
        } else if (curr_token.tokenType == TokenType::kEntityProcedure) {
            int next_index = procedureParser->parse(tokens, curr_index);

            if (next_index == -1) {
                throw InvalidSyntaxError();
            } else {
                curr_index = next_index;
            }
        } else if (curr_token.tokenType == TokenType::kEntityRead) {
            readParser->lineNumber = lineNumber;
            int next_index = readParser->parse(tokens, curr_index);

            if (next_index == -1) {
                throw InvalidSyntaxError();
            } else {
                lineNumber++;
                curr_index = next_index;
            }
        } else if (curr_token.tokenType == TokenType::kEntityPrint) {
            printParser->lineNumber = lineNumber;
            int next_index = printParser->parse(tokens, curr_index);
            if (next_index == -1) {
                throw InvalidSyntaxError();
            } else {
                lineNumber++;
                curr_index = next_index;
            }
        } else if (curr_token.tokenType == TokenType::kEntityWhile) {
            std::cout << "Test" << std::endl;  // validate if the loop is entered
            int next_index = whileParser->parse(tokens, curr_index);

            if (next_index == -1) {
                throw InvalidSyntaxError();
            } else {
                lineNumber++;
                curr_index = next_index;
            }
        } else if (curr_token.tokenType == TokenType::kEntityIf) {  // might need special handling
            int next_index = ifParser->parse(tokens, curr_index);

            if (next_index == -1) {
                throw InvalidSyntaxError();
            } else {
                lineNumber++;
                curr_index = next_index;
            }
        } else {
            // currently unsupported, skip line for now
            int temp = curr_index;

            while (tokens[temp].tokenType != TokenType::kSepSemicolon &&
                tokens[temp].tokenType != TokenType::kSepOpenBrace) {
                temp++;
            }

            lineNumber++;
            curr_index = temp + 1;
        }
    }

    // <line, RHS patterns>, <line, LHS var>
    writeFacade->storeAssignments(visitor->getUsesLineRHSPatternMap(), visitor->getUsesLineLHSMap());
    // <all var in LHS and RHS>
    writeFacade->storeVariables(visitor->getVariables());
    // <all const in RHS>
    writeFacade->storeConstants(visitor->getConstants());
    // <all var in RHS>
//    writeFacade->storeUses(visitor->getUsesLineRHSVarMap());
    return curr_index;
}
