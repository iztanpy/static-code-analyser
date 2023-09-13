#include "PKB/API/WriteFacade.h"
#include <unordered_map>


WriteFacade::WriteFacade(PKB* pkbptr) : pkb(pkbptr) {}

void WriteFacade::storeAssignments(std::unordered_set<int> assignments) {
}

void WriteFacade::storeVariables(std::unordered_set<std::string> variables) {
}

void WriteFacade::storeConstants(std::unordered_set<std::string> constants) {
}

void WriteFacade::storeUsesConst(std::unordered_map<std::string, std::unordered_set<std::string>> LHSAndRHS) {
}
void WriteFacade::storeUsesVar(std::unordered_map<std::string, std::unordered_set<std::string>> LHSAndRHS) {
}

void WriteFacade::storeLineUses(std::unordered_map<int, std::unordered_set<std::string>> usesVariableAndConstants) {
}
