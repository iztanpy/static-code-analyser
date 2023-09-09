#include "PKB/API/WriteFacade.h"

WriteFacade::WriteFacade(PKB* pkbptr) : pkb(pkbptr) {}

void WriteFacade::storeAssignments(std::vector<int> assignments) {
    pkb->setAssignments(assignments);
}

void WriteFacade::storeVariables(std::vector<std::string> variables) {
    pkb->setVariables(variables);
}

void WriteFacade::storeConstants(std::vector<std::string> constants) {
    pkb->setConstants(constants);
}

void WriteFacade::storeUsesConst(std::unordered_map<std::string, std::unordered_set<std::string>> LHSAndRHS) {
    pkb->setUsesConst(LHSAndRHS);
};
void WriteFacade::storeUsesVar(std::unordered_map<std::string, std::unordered_set<std::string>> LHSAndRHS) {
    pkb->setUsesVar(LHSAndRHS);
};

void WriteFacade::storeLineUses(std::unordered_map<int, std::unordered_set<std::string>> usesVariableAndConstants){
    pkb->setLineUses(usesVariableAndConstants);
};