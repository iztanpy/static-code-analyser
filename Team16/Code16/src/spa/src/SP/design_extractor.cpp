#include "SP/design_extractor.h"

void design_extractor::extractDesign(std::shared_ptr<TNode> root, ASTVisitor* visitor) {
  std::string key;
  root->accept(visitor, key);
}

void design_extractor::populatePKB(ASTVisitor* visitor, write_facade* writeFacade) {
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
  writeFacade->storeNext(cfg::getNextStatementNumberHashmap());
  // Store CFG root nodes
  writeFacade->storeCfg(cfg::getCfgNodeHashmap());
  // Store CFG legend
  writeFacade->storeCfgLegend(cfg::getStmtNumberToCfgNodeHashmap());
}
