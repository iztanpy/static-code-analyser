#include "PKB/API/WriteFacade.h"
#include <unordered_map>


WriteFacade::WriteFacade(PKB* pkbptr) : pkb(pkbptr) {}

void WriteFacade::storeRHSAssignments( std::unordered_map<statementNumber, std::unordered_set<possibleCombinations>>assignments) {
    pkb->assignStore->addNumRHSMap(assignments);
}

void WriteFacade::storeLHSAssignments(std::unordered_map<statementNumber, variable> numLHSMap) {
    pkb->assignStore->addNumLHSMap(numLHSMap);
}

void WriteFacade::storeVariables(std::unordered_set<std::string> variables) {
    pkb->variableStore->addVariables(variables);
}

void WriteFacade::storeConstants(std::unordered_set<std::string> constants) {
    pkb->constantStore->addConstants(constants);
}

void WriteFacade::addLineUsesVar(std::unordered_map<statementNumber, std::unordered_set<variable>> varUsesMap) {
    pkb->usesStore-> addLineUsesVar(varUsesMap);
}
void WriteFacade::addLineUsesConst(std::unordered_map<statementNumber, std::unordered_set<constant>> constUsesMap) {
    pkb->usesStore->addLineUsesVar(constUsesMap);
}

