#include "PKB/API/write_facade.h"

write_facade::write_facade(PKB& pkb) : pkb(pkb) {}

void write_facade::storeAssignments(std::unordered_map<statementNumber,
                                                       std::unordered_set<possibleCombinations>> numRHSMap,
                                    std::unordered_map<statementNumber, variable> numLHSMap) {
  pkb.setAssignments(numRHSMap, numLHSMap);
}

void write_facade::storeAssignments(std::unordered_map<statementNumber,
                                                       std::unordered_set<partialMatch>> partialRHSMap,
                                    std::unordered_map<statementNumber, full> fullRHSMap,
                                    std::unordered_map<statementNumber, variable> numLHSMap) {
  pkb.setAssignments(partialRHSMap, fullRHSMap, numLHSMap);
}

void write_facade::storeVariables(std::unordered_set<std::string> variables) {
  pkb.addVariables(variables);
}

void write_facade::storeProcedures(std::set<std::string> procedures) {
  pkb.addProcedures(procedures);
}

void write_facade::storeProcedures(std::unordered_map<procedure, std::pair<int, int>> procedures) {
  pkb.addProcedures(procedures);
  pkb.storeUsesProcedures(procedures, pkb.getCallStar());
  pkb.storeModifiesProcedures(procedures, pkb.getCallStar());
}

void write_facade::storeUses(std::unordered_map<statementNumber, std::unordered_set<variable>> varUsesMap) {
  pkb.storeUses(varUsesMap);
}

void write_facade::storeConstants(std::unordered_set<constant> constants) {
  pkb.addConstants(constants);
}

void write_facade::storeStatements(std::unordered_map<statementNumber, StmtEntity> typeMap) {
  pkb.addStatements(typeMap);
}

void write_facade::storeParent(std::unordered_map<statementNumber, std::unordered_set<statementNumber>> map) {
  pkb.storeParent(map);
}

void write_facade::storeFollows(std::unordered_map<statementNumber, statementNumber> map) {
  pkb.storeFollows(map);
}

void write_facade::storeCalls(std::unordered_map<procedure, std::unordered_set<procedure>> callTable) {
  pkb.storeCalls(callTable);
}

void write_facade::storeCallStatements(std::unordered_map<statementNumber, procedure> callStatements) {
  pkb.storeModifiesCalls(callStatements);
  pkb.storeUsesCalls(callStatements);
  pkb.storeCallsPairs(callStatements);
}

void write_facade::storeModifies(std::unordered_map<statementNumber, variable> varModifiesMap) {
  pkb.storeModifies(varModifiesMap);
}

void write_facade::storeWhile(std::unordered_map<statementNumber, std::unordered_set<variable>> variableMap) {
  pkb.storeWhile(variableMap);
}

void write_facade::storeIf(std::unordered_map<statementNumber, std::unordered_set<variable>> variableMap) {
  pkb.storeIf(variableMap);
}

void write_facade::storeNext(std::unordered_map<statementNumber, std::unordered_set<statementNumber>> NextMap) {
  pkb.storeNext(NextMap);
}

void write_facade::storeCfg(std::unordered_map<std::string, std::shared_ptr<cfg_node>> cfgRoots) {
  pkb.storeCfg(cfgRoots);
}

void write_facade::storeCfgLegend(std::unordered_map<statementNumber, std::shared_ptr<cfg_node>> cfgLegend) {
  pkb.storeCfgLegend(cfgLegend);
}

