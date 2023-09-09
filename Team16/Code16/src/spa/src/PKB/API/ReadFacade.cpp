#include <stdio.h>
#include <iostream>
#include <vector>

#include "PKB/API/ReadFacade.h"

ReadFacade::ReadFacade(PKB* pkbptr) : pkb(pkbptr) { }

std::vector<int> ReadFacade::getAllAssigns() {
    return pkb->getAssignments();
}

std::vector<std::string> ReadFacade::getAllVariables() {
    return pkb->getVariables();
}

std::vector<std::string> ReadFacade::getAllConstants() {
    return pkb->getConstants();
}

std::unordered_set<std::string> ReadFacade::getVariablesUsedBy(int lineNumber) {
    return pkb->getVariablesUsedBy(lineNumber);
}
