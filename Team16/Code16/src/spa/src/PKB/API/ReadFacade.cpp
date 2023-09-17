#include <iostream>
#include <unordered_set>

#include "PKB/API/ReadFacade.h"

ReadFacade::ReadFacade(PKB& pkb) : pkb(pkb) {}

std::unordered_set<statementNumber> ReadFacade::getAllAssigns() {
    return pkb.getAllAssigns();
}

std::unordered_set<statementNumber> ReadFacade::getAssigns(variable LHS, possibleCombinations RHS) {
    return pkb.getAssigns(LHS, RHS);
}

std::unordered_set<variable> ReadFacade::getAllVariables() {
    return pkb.getVariables();
}

std::unordered_set<variable> ReadFacade::getVariablesUsedBy(statementNumber line) {
    return pkb.getVariablesUsedBy(line);
}

std::unordered_set<std::string> ReadFacade::getAllConstants() {
    return pkb.getConstants();
}
