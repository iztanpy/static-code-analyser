#include <stdio.h>
#include <iostream>
#include <unordered_set>

#include "PKB/API/ReadFacade.h"

ReadFacade::ReadFacade(PKB* pkbptr) : pkb(pkbptr) { }

std::unordered_set<int> ReadFacade::getAllAssigns() {
    return pkb->getAssignments();
}

std::unordered_set<std::string> ReadFacade::getAllVariables() {
    return pkb->getVariables();
}

std::unordered_set<std::string> ReadFacade::getAllConstants() {
    return pkb->getConstants();
}

std::unordered_set<std::string> ReadFacade::getVariablesUsedBy(int lineNumber) {
    return pkb->getVariablesUsedBy(lineNumber);
}
