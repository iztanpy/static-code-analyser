#include <iostream>
#include <vector>
#include "SimpleParser.h"

SimpleParser::SimpleParser(WriteFacade* writeFacadePtr, ASTVisitor* astVisitorPtr) : writeFacade(writeFacadePtr),
visitor(astVisitorPtr) {}

int SimpleParser::parse(std::vector<Token>& tokens) {
    reset();

    while (index < tokens.size()) {
        Token* curr_token = &tokens[index];
        bool isWhileParent = !controlStructureStack.empty() && controlStructureStack.top() == "while";
        bool isIfParent = !controlStructureStack.empty() && (
            controlStructureStack.top() == "if");
        int parentStatementNumber = !parentStatementStack.empty() ? parentStatementStack.top() : -1;
        if ((isWhileParent || isIfParent) && parentStatementNumber != -1
            && curr_token->tokenType != TokenType::kSepCloseBrace) {
            visitor->setParentStatementNumberMap(parentStatementNumber, lineNumber);
        }

        if (followsStatementStack.empty() && tokens[index].getValue() != "procedure") {
            throw InvalidSyntaxError();
        }
        parser = factory->createParser(curr_token->tokenType, lineNumber, index);
        parser->start_parse(tokens, index);
    }
    populatePKB();
    return index;
}

void SimpleParser::populatePKB() {
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
  writeFacade->storeStatements(visitor->getStatementTypesMap());
  // Store Procedures
  writeFacade->storeProcedures(visitor->getProcedureLabels());
}
