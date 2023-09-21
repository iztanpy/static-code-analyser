#include <iostream>
#include <vector>
#include "SimpleParser.h"

SimpleParser::SimpleParser(WriteFacade* writeFacadePtr, ASTVisitor* astVisitorPtr) : writeFacade(writeFacadePtr),
visitor(astVisitorPtr), currWhileDepth(0), currIfDepth(0) {}

int SimpleParser::parse(const std::vector<Token>& tokens, int curr_index) {
    std::unordered_map<int, std::unordered_set<int>> parentStatementNumberHashmap;
    std::stack<std::string> controlStructureStack;  // Track the current control structures (if, else, while)
    std::stack<int> parentStatementStack;  // Track the parent statement lines
    while (curr_index < tokens.size()) {
        Token curr_token = tokens[curr_index];
        bool isWhileParent = !controlStructureStack.empty() && controlStructureStack.top() == "while";
        bool isIfParent = !controlStructureStack.empty() && (
            controlStructureStack.top() == "if" || controlStructureStack.top() == "else");
        int parentStatementNumber = !parentStatementStack.empty() ? parentStatementStack.top() : -1;
        if ((isWhileParent || isIfParent) && parentStatementNumber != -1
            && curr_token.tokenType != TokenType::kSepCloseBrace) {
            visitor->setParentStatementNumberMap(parentStatementNumber, lineNumber);
        }

        if (curr_token.tokenType == TokenType::kLiteralName) {
            Token next_token = tokens.at(curr_index + 1);

            if (next_token.tokenType == TokenType::kEntityAssign) {
                assignmentParser->lineNumber = lineNumber;
                assignmentParser->index = curr_index;
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
            controlStructureStack.push("while");
            parentStatementStack.push(lineNumber);
            whileParser->lineNumber = lineNumber;
            whileParser->index = curr_index;
            int next_index = whileParser->parse(tokens, curr_index);
            currWhileDepth++;

            if (next_index == -1) {
                throw InvalidSyntaxError();
            } else {
                lineNumber++;
                curr_index = next_index;
            }
        } else if (curr_token.tokenType == TokenType::kEntityCall) {
            int next_index = callParser->parse(tokens, curr_index);
            if (next_index == -1) {
                throw InvalidSyntaxError();
            } else {
                lineNumber++;
                curr_index = next_index;
            }
        } else if (curr_token.tokenType == TokenType::kEntityIf) {
            controlStructureStack.push("if");
            parentStatementStack.push(lineNumber);
            int next_index = ifParser->parse(tokens, curr_index);
            currIfDepth++;

            if (next_index == -1) {
                throw InvalidSyntaxError();
            } else {
                lineNumber++;
                nestingLevel++;
                curr_index = next_index;
            }
        } else if (curr_token.tokenType == TokenType::kEntityElse) {
            if (!controlStructureStack.empty() && controlStructureStack.top() == "if") {
                curr_index += 2;  // skip over the next open brace
            } else {
                // Error: Unexpected 'else' without matching 'if'
                throw InvalidSyntaxError();
            }
        } else if (curr_token.tokenType == TokenType::kSepCloseBrace) {
            if (!controlStructureStack.empty() && controlStructureStack.top() == "while" && currWhileDepth >= 1) {
                controlStructureStack.pop();  // Pop the 'while'
                parentStatementStack.pop();  // Pop the parent statement
                currWhileDepth--;  // Decrease the depth
                nestingLevel--;
            } else if (!controlStructureStack.empty() && controlStructureStack.top() == "if" && currIfDepth >= 1) {
                if (curr_index + 1 < tokens.size() && tokens[curr_index + 1].tokenType != TokenType::kEntityElse) {
                    currIfDepth--;  // Decrease the depth
                    nestingLevel--;
                    controlStructureStack.pop();  // Pop the 'if'
                    parentStatementStack.pop();  // Pop the parent
                }
            } else {  // other cases which have brackets
                if (!controlStructureStack.empty() && currWhileDepth > 0) {
                    currWhileDepth--;  // Decrease the depth
                    currIfDepth--;  // Decrease the depth
                }
            }
            curr_index += 1;
        }  else {
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
    //    writeFacade->storeUses(visitor->getUsesLineRHSVarMap
    writeFacade->storeLineUses(visitor->getUsesStatementNumberHashmap());
    return curr_index;
}
