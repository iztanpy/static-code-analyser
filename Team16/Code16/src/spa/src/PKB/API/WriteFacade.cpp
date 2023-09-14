#include <unordered_map>

#include "PKB/API/WriteFacade.h"

WriteFacade::WriteFacade(PKB& pkb) : pkb(pkb) {}

void WriteFacade::storeAssignments(std::unordered_set<int> assignments) {
    pkb.setAssignments(assignments);
}

void WriteFacade::storeVariables(std::unordered_set<std::string> variables) {
    pkb.setVariables(variables);
}

void WriteFacade::storeConstants(std::unordered_set<std::string> constants) {
    pkb.setConstants(constants);
}

void WriteFacade::storeUsesConst(std::unordered_map<std::string, std::unordered_set<std::string>> LHSAndRHS) {
    pkb.setUsesConst(LHSAndRHS);
}
void WriteFacade::storeUsesVar(std::unordered_map<std::string, std::unordered_set<std::string>> LHSAndRHS) {
    pkb.setUsesVar(LHSAndRHS);
}

void WriteFacade::storeLineUses(std::unordered_map<int, std::unordered_set<std::string>> usesVariableAndConstants) {
    pkb.setLineUses(usesVariableAndConstants);
}
