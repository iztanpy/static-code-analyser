#include <unordered_map>
#include <set>

#include "PKB/API/WriteFacade.h"

WriteFacade::WriteFacade(PKB& pkb) : pkb(pkb) {}


void WriteFacade::storeAssignments(std::unordered_map<statementNumber,
    std::unordered_set<possibleCombinations>> numRHSMap, std::unordered_map<statementNumber, variable> numLHSMap) {
    pkb.setAssignments(numRHSMap, numLHSMap);
}

void WriteFacade::storeAssignments(std::unordered_map<statementNumber,
                                   std::unordered_set<partialMatch>> partialRHSMap,
                                   std::unordered_map<statementNumber, full> fullRHSMap,
                                   std::unordered_map<statementNumber, variable> numLHSMap) {
    pkb.setAssignments(partialRHSMap, fullRHSMap, numLHSMap);
}

void WriteFacade::storeVariables(std::unordered_set<std::string> variables) {
    pkb.addVariables(variables);
}

void WriteFacade::storeProcedures(std::set<std::string> procedures) {
    pkb.addProcedures(procedures);
}

void WriteFacade::storeProcedures(std::unordered_map<procedure, std::pair<int, int>> procedures) {
    pkb.addProcedures(procedures);
    pkb.storeUsesProcedures(procedures, pkb.getCallStar());
    pkb.storeModifiesProcedures(procedures, pkb.getCallStar());
}

void WriteFacade::storeUses(std::unordered_map<statementNumber, std::unordered_set<variable>> varUsesMap) {
    pkb.storeUses(varUsesMap);
}

void WriteFacade::storeConstants(std::unordered_set<constant> constants) {
    pkb.addConstants(constants);
}

void WriteFacade::storeStatements(std::unordered_map<statementNumber, StmtEntity> typeMap) {
    pkb.addStatements(typeMap);
}

void WriteFacade::storeParent(std::unordered_map<statementNumber, std::unordered_set<statementNumber>> map) {
    pkb.storeParent(map);
}

void WriteFacade::storeFollows(std::unordered_map<statementNumber, statementNumber> map) {
    pkb.storeFollows(map);
}

void WriteFacade::storeCalls(std::unordered_map<procedure, std::unordered_set<procedure>> callTable) {
    pkb.storeCalls(callTable);
}

void WriteFacade::storeModifies(std::unordered_map<statementNumber, variable> varModifiesMap) {
    pkb.storeModifies(varModifiesMap);
}

void WriteFacade::storeWhile(std::unordered_map<statementNumber, std::unordered_set<variable>> variableMap) {
    pkb.storeWhile(variableMap);
}

void WriteFacade::storeIf(std::unordered_map<statementNumber, std::unordered_set<variable>> variableMap) {
    pkb.storeIf(variableMap);
}

