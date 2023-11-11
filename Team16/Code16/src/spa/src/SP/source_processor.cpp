#include "SP/source_processor.h"

SourceProcessor::SourceProcessor(write_facade* writeFacadePtr) :
    writeFacade(writeFacadePtr), simpleParser(&this->visitor) {}

void SourceProcessor::processSource(const std::string& simpleProgram) {
  std::vector<struct Token> tokens = tokeniser.tokenise(simpleProgram);
  try {
    simpleParser.parse(tokens);
    populatePKB();
  } catch (const InvalidSyntaxError& e) {
    e.log();
  } catch (const InvalidTokenTypeError& e) {
    e.log();
  } catch (const InvalidSemanticError& e) {
    e.log();
  }
}

std::unordered_set<std::string> SourceProcessor::getVariables() {
  return visitor.getVariables();
}

std::unordered_set<std::string> SourceProcessor::getConstants() {
  return visitor.getConstants();
}

std::unordered_map<int, StmtEntity> SourceProcessor::getStatementTypesMap() {
  return visitor.getStatementTypesMap();
}

std::unordered_map<int, std::unordered_set<std::string>> SourceProcessor::getAssignLinePartialRHSPatternMap() {
  return visitor.getAssignLinePartialRHSPatternMap();
}

std::unordered_map<int, std::string> SourceProcessor::getUsesLineLHSMap() {
  return visitor.getUsesLineLHSMap();
}

std::unordered_map<int, std::unordered_set<std::string>> SourceProcessor::getUsesLineRHSVarMap() {
  return visitor.getUsesLineRHSVarMap();
}

std::unordered_map<int, std::string> SourceProcessor::getAssignLineFullRHSMap() {
  return visitor.getAssignLineFullRHSMap();
}

std::unordered_map<int, std::unordered_set<std::string>> SourceProcessor::getWhileControlVarMap() {
  return visitor.getWhileControlVarMap();
}

std::unordered_map<int, std::unordered_set<std::string>> SourceProcessor::getIfControlVarMap() {
  return visitor.getIfControlVarMap();
}
std::unordered_map<int, std::string> SourceProcessor::getModifiesMap() {
  return visitor.getModifiesMap();
}

std::unordered_map<int, std::unordered_set<int>> SourceProcessor::getParentStatementNumberMap() {
  return visitor.getParentStatementNumberMap();
}

std::unordered_map<int, int> SourceProcessor::getFollowStatementNumberMap() {
  return visitor.getFollowStatementNumberMap();
}

std::set<std::string> SourceProcessor::getProcedureLabels() {
  return visitor.getProcedureLabels();
}

std::unordered_map<std::string, std::pair<int, int>> SourceProcessor::getProcedureLineNumberHashmap() {
  return visitor.getProcedureLineNumberHashmap();
}

std::unordered_map<std::string, std::unordered_set<std::string>> SourceProcessor::getCallerCalleeHashmap() {
  return visitor.getCallerCalleeHashmap();
}

std::unordered_map<int, std::string> SourceProcessor::getCallStatementNumberEntityHashmap() {
  return visitor.getCallStatementNumberEntityHashmap();
}

void SourceProcessor::populatePKB() {
  // Store Parent <line, set<line>>
  writeFacade->storeParent(visitor.getParentStatementNumberMap());
  // Store Assignments: <line, RHS patterns>, <line, full RHS>, <line, LHS var>
  writeFacade->storeAssignments(visitor.getAssignLinePartialRHSPatternMap(),
                                visitor.getAssignLineFullRHSMap(),
                                visitor.getUsesLineLHSMap());
  // Store Calls
  writeFacade->storeCalls(visitor.getCallerCalleeHashmap());
  // Store Procedures
  writeFacade->storeProcedures(visitor.getProcedureLabels());
  // Store Uses: <all var in RHS>
  writeFacade->storeUses(visitor.getUsesLineRHSVarMap());
  // Store Modifies: <line, var>
  writeFacade->storeModifies(visitor.getModifiesMap());
  // Store Procedures line numbers
  writeFacade->storeProcedures(visitor.getProcedureLineNumberHashmap());
  // Store call statements and the procedures they call
  writeFacade->storeCallStatements(visitor.getCallStatementNumberEntityHashmap());
  // Store Follows <line, line>
  writeFacade->storeFollows(visitor.getFollowStatementNumberMap());
  // Store Variables <all var in LHS and RHS>
  writeFacade->storeVariables(visitor.getVariables());
  // Store Constants <all const in RHS>
  writeFacade->storeConstants(visitor.getConstants());
  // Store Statement types
  writeFacade->storeStatements(visitor.getStatementTypesMap());
  // Store while control variables
  writeFacade->storeWhile(visitor.getWhileControlVarMap());
  // Store if control variables
  writeFacade->storeIf(visitor.getIfControlVarMap());
  // Store Next
  writeFacade->storeNext(getNextStatementMap());
  //   Store CFG root nodes
  writeFacade->storeCfg(getCfgNodesMap());
  //  Store CFG legend
  writeFacade->storeCfgLegend(getStmtNumberToCfgNodeHashmap());
}
