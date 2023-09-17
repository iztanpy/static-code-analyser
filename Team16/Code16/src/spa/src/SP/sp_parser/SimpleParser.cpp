#include <iostream>
#include <vector>
#include "SimpleParser.h"

SimpleParser::SimpleParser(WriteFacade* writeFacadePtr) : writeFacade(writeFacadePtr) { }
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
            int next_index = readParser->parse(tokens, curr_index);

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

    writeFacade->storeAssignments(
        assignmentParser->getUsesStatementNumberHashmap(),
        assignmentParser->getUsesStatementNumberVarHashmap());
    writeFacade->storeVariables(assignmentParser->getVariablesHashset());
    writeFacade->storeConstants(assignmentParser->getConstantsHashset());
    writeFacade->storeUsesVar(assignmentParser->getAssignVarHashmap());
    writeFacade->storeUsesConst(assignmentParser->getAssignConstHashmap());
    writeFacade->storeLineUses(assignmentParser->getUsesStatementNumberHashmap());

    return curr_index;
}

void SimpleParser::tokenise(std::string code) {
    std::vector<struct Token> tokens = tokeniser.tokenise(code);
    try { parse(tokens, 0); }
    catch (const InvalidSyntaxError& e) {
        e.log();
    }
}
