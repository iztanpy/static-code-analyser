#include <stdio.h>
#include <iostream>
#include <unordered_set>

#include "PKB/API/ReadFacade.h"

ReadFacade::ReadFacade(PKB* pkbptr) : pkb(pkbptr) {}

std::unordered_set<int> ReadFacade::GetAllAssigns() {
    return pkb->getAssignments();
}

std::unordered_set<std::string> ReadFacade::GetAllVariables() {
    return pkb->getVariables();
}

std::unordered_set<std::string> ReadFacade::GetAllConstants() {
    return pkb->getConstants();
}

std::unordered_set<std::string> ReadFacade::GetVariablesUsedBy(int line_number) {
    return pkb->getVariablesUsedBy(line_number);
}
