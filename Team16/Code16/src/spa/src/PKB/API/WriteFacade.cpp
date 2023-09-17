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

void WriteFacade::storeUses(std::unordered_map<statementNumber, std::unordered_set<variable>> varUsesMap) {
    pkb.storeUses(varUsesMap);
}

void WriteFacade::storeConstants(std::unordered_set<constant> constants) {
    pkb.addConstants(constants);
}
