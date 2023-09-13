#include <stdio.h>
#include <iostream>
#include <unordered_set>

#include "PKB/API/ReadFacade.h"

ReadFacade::ReadFacade(PKB* pkbptr) : pkb(pkbptr) {}

std::unordered_set<statementNumber> ReadFacade::getAllAssigns() {
    return pkb->getAllAssigns();
}

std::unordered_set<statementNumber> ReadFacade::getAllAssigns(variable LHS, possibleCombinations RHS) {
    return pkb->getAssigns(LHS, RHS);
}

std::unordered_set<variable> ReadFacade::getAllVariables() {
    return pkb->getVariables();
}

std::unordered_set<variable> ReadFacade::getVariablesUsedBy(statementNumber line) {
    return pkb ->getVariablesUsedBy(line);
}

std::unordered_set<constant> ReadFacade::getConstantsUsedBy(statementNumber line) {
    return pkb -> getConstantsUsedBy(line);
}

std::unordered_set<std::string> ReadFacade::getAllConstants() {
    return pkb -> getConstants();
}
