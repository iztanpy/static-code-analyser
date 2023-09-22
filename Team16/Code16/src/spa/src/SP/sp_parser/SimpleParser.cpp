#include <iostream>
#include <vector>
#include "SimpleParser.h"

SimpleParser::SimpleParser(WriteFacade* writeFacadePtr, ASTVisitor* astVisitorPtr) : writeFacade(writeFacadePtr),
visitor(astVisitorPtr), currWhileDepth(0), currIfDepth(0) {}

int SimpleParser::parse(const std::vector<Token>& tokens, int curr_index) {
    std::unordered_map<int, std::unordered_set<int>> parentStatementNumberHashmap;
    std::stack<std::string> controlStructureStack;  // Track the current control structures (if, else, while)
    std::stack<int> parentStatementStack;  // Track the parent statement lines
    std::stack<std::set<int>> followsStatementStack;

    while (curr_index < tokens.size()) {
        Token curr_token = tokens[curr_index];
        bool isWhileParent = !controlStructureStack.empty() && controlStructureStack.top() == "while";
        bool isIfParent = !controlStructureStack.empty() && (
            controlStructureStack.top() == "if");
        int parentStatementNumber = !parentStatementStack.empty() ? parentStatementStack.top() : -1;
        if ((isWhileParent || isIfParent) && parentStatementNumber != -1
            && curr_token.tokenType != TokenType::kSepCloseBrace) {
            visitor->setParentStatementNumberMap(parentStatementNumber, lineNumber);
        }

        if (followsStatementStack.empty() && tokens[curr_index].tokenType != TokenType::kEntityProcedure) {
            throw InvalidSyntaxError();
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
                    followsStatementStack.top().insert(lineNumber);
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
            std::set<int> procedureFollowsSet;
            followsStatementStack.push(procedureFollowsSet);
        } else if (curr_token.tokenType == TokenType::kEntityRead) {
            readParser->lineNumber = lineNumber;
            int next_index = readParser->parse(tokens, curr_index);
            if (next_index == -1) {
                throw InvalidSyntaxError();
            } else {
                followsStatementStack.top().insert(lineNumber);
                lineNumber++;
                curr_index = next_index;
            }
        } else if (curr_token.tokenType == TokenType::kEntityPrint) {
            printParser->lineNumber = lineNumber;
            int next_index = printParser->parse(tokens, curr_index);
            if (next_index == -1) {
                throw InvalidSyntaxError();
            } else {
                followsStatementStack.top().insert(lineNumber);
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
                followsStatementStack.top().insert(lineNumber);
                std::set<int> whileFollowsSet;
                followsStatementStack.push(whileFollowsSet);
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
            ifParser->lineNumber = lineNumber;
            ifParser->index = curr_index;
            int next_index = ifParser->parse(tokens, curr_index);
            currIfDepth++;

            if (next_index == -1) {
                throw InvalidSyntaxError();
            } else {
                followsStatementStack.top().insert(lineNumber);
                std::set<int> ifFollowsSet;
                followsStatementStack.push(ifFollowsSet);
                lineNumber++;

                curr_index = next_index;
            }
        } else if (curr_token.tokenType == TokenType::kEntityElse) {
            if (!controlStructureStack.empty() && controlStructureStack.top() == "if") {
                std::set<int> elseFollowsSet;
                followsStatementStack.push(elseFollowsSet);
                curr_index += 2;  // skip over the next open brace
            } else {
                // Error: Unexpected 'else' without matching 'if'
                throw InvalidSyntaxError();
            }
        } else if (curr_token.tokenType == TokenType::kSepCloseBrace) {
          std::set<int> top_set = followsStatementStack.top();
          insertFollowsHashMap(top_set);
          followsStatementStack.pop();
          if (!controlStructureStack.empty() && controlStructureStack.top() == "while" && currWhileDepth >= 1) {
                controlStructureStack.pop();  // Pop the 'while'
                parentStatementStack.pop();  // Pop the parent statement
                currWhileDepth--;  // Decrease the depth

            } else if (!controlStructureStack.empty() && controlStructureStack.top() == "if" && currIfDepth >= 1) {
                if (curr_index + 1 < tokens.size() && tokens[curr_index + 1].tokenType != TokenType::kEntityElse) {
                    currIfDepth--;  // Decrease the depth
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
    // Store Parent <line, set<line>>
    writeFacade->storeParent(visitor->getParentStatementNumberMap());
    // Store Assignments: <line, RHS patterns>, <line, LHS var>
    writeFacade->storeAssignments(visitor->getUsesLineRHSPatternMap(), visitor->getUsesLineLHSMap());
    // Store Uses: <all var in RHS>
    writeFacade->storeUses(visitor->getUsesLineRHSVarMap());
    // Store Modifies: <line, var>
    writeFacade->storeModifies(visitor->getModifiesMap());
    // Store Follows <line, line>
    writeFacade->storeFollows(visitor->getFollowStatementNumberMap());
    // Store Variables <all var in LHS and RHS>
    writeFacade->storeVariables(visitor->getVariables());
    // Store Constants <all const in RHS>
    writeFacade->storeConstants(visitor->getConstants());
    // Store Statement types
//    writeFacade->storeStatements(visitor->getStatementTypesMap());
    return curr_index;
}


void SimpleParser::insertFollowsHashMap(std::set<int> followsSet) {
  if (followsSet.size() > 1) {
    auto it = followsSet.begin();
    auto nextIt = std::next(it);

    // Iterate up to the second-to-last element
    while (nextIt != followsSet.end()) {
      // Compare *it and *nextIt
      int before = *it;
      int after = *nextIt;
      visitor->setFollowStatementNumberMap(before, after);

      ++it;
      ++nextIt;
    }
  }
}
