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
            controlStructureStack.top() == "if" || controlStructureStack.top() == "else");
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
    checkCalls();
    populatePKB();
    return index;
}

void SimpleParser::checkCalls() {
    for (const auto& callerCallee : visitor->getCallerCalleeHashmap()) {
        const std::unordered_set<std::string>& calleeSet = callerCallee.second;
        for (const std::string& callee : calleeSet) {
            // Check if callee exists in procedureLabels
            std::string test = callee;
            std::set<std::string> procLabels = visitor->getProcedureLabels();
            int count = procLabels.count(callee);
            if (procLabels.count(callee) == 0) {
                throw InvalidSemanticError();
            }
        }
    }
}

void SimpleParser::populatePKB() {
  // Store Parent <line, set<line>>
  writeFacade->storeParent(visitor->getParentStatementNumberMap());
  // Store Assignments: <line, RHS patterns>, <line, full RHS>, <line, LHS var>
  writeFacade->storeAssignments(visitor->getAssignLinePartialRHSPatternMap(),
                                visitor->getAssignLineFullRHSMap(),
                                visitor->getUsesLineLHSMap());
  // Store Calls
  writeFacade->storeCalls(visitor->getCallerCalleeHashmap());
  // Store Procedures
  writeFacade->storeProcedures(visitor->getProcedureLabels());
  // Store Uses: <all var in RHS>
  writeFacade->storeUses(visitor->getUsesLineRHSVarMap());
  // Store Modifies: <line, var>
  writeFacade->storeModifies(visitor->getModifiesMap());
  // Store Procedures line numbers
  writeFacade->storeProcedures(visitor->getProcedureLineNumberHashmap());
  // Store call statements and the procedures they call
  writeFacade->storeCallStatements(visitor->getCallStatementNumberEntityHashmap());
  // Store Follows <line, line>
  writeFacade->storeFollows(visitor->getFollowStatementNumberMap());
  // Store Variables <all var in LHS and RHS>
  writeFacade->storeVariables(visitor->getVariables());
  // Store Constants <all const in RHS>
  writeFacade->storeConstants(visitor->getConstants());
  // Store Statement types
  writeFacade->storeStatements(visitor->getStatementTypesMap());
  // Store while control variables
  writeFacade->storeWhile(visitor->getWhileControlVarMap());
  // Store if control variables
  writeFacade->storeIf(visitor->getIfControlVarMap());
  // Store Next
  writeFacade->storeNext(getNextStatementMap());
//   Store CFG root nodes
//  writeFacade->storeCfg(getCfgNodesMap());
//   Store CFG legend
  writeFacade->storeCfgLegend(getStmtNumberToCfgNodeHashmap());
}
