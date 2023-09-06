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
