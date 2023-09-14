#include <unordered_map>

#include "PKB/API/WriteFacade.h"

WriteFacade::WriteFacade(PKB& pkb) : pkb(pkb) {}


void WriteFacade::storeAssignments(std::unordered_map<statementNumber,
    std::unordered_set<possibleCombinations>> numRHSMap, std::unordered_map<statementNumber, variable> numLHSMap) {
    pkb.setAssignments(numRHSMap, numLHSMap);
}

void WriteFacade::storeVariables(std::unordered_set<std::string> variables) {
    pkb.addVariables(variables);
}

void WriteFacade::addLineUsesVar(std::unordered_map<statementNumber, std::unordered_set<variable>> varUsesMap) {
    pkb.addLineUsesVar(varUsesMap);
}

void WriteFacade::addLineUsesConst(std::unordered_map<statementNumber, std::unordered_set<constant>> constUsesMap) {
    pkb.addLineUsesConst(constUsesMap);
}

void WriteFacade::storeConstants(std::unordered_set<constant> constants) {
    pkb.addConstants(constants);
}

void WriteFacade::storeUsesConst(std::unordered_map<std::string, std::unordered_set<std::string>> LHSAndRHS) {
}
void WriteFacade::storeUsesVar(std::unordered_map<std::string, std::unordered_set<std::string>> LHSAndRHS) {
}

void WriteFacade::storeLineUses(std::unordered_map<int, std::unordered_set<std::string>> usesVariableAndConstants) {
}
