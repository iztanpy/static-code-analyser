#include<stdio.h>
#include <iostream>
#include <string>
#include <unordered_set>
#include <memory>

#include "PKB.h"


PKB::PKB() {
    assignStore = std::make_unique<AssignStore>();
    variableStore = std::make_unique<VariableStore>();
    usesStore = std::make_unique<UsesStore>();
    constantStore = std::make_unique<ConstantStore>();
}

void PKB::setAssignments(std::unordered_map<statementNumber,
    std::unordered_set<possibleCombinations>> numRHSMap, std::unordered_map<statementNumber, variable> numLHSMap) {
    assignStore->addNumLHSMap(numLHSMap);
    assignStore->addNumRHSMap(numRHSMap);
}

std::unordered_set<statementNumber> PKB::getAllAssigns() {
    return assignStore->getAllAssigns();
}

std::unordered_set<statementNumber> PKB::getAssigns(variable LHS, possibleCombinations RHS) {
    return assignStore->getAssigns(LHS, RHS);
}

void PKB::addVariables(std::unordered_set<variable> variables) {
    variableStore->addVariables(variables);
}

std::unordered_set<variable> PKB::getVariables() {
    return variableStore->getVariables();
}

void PKB::storeUses(std::unordered_map<statementNumber, std::unordered_set<variable>> varUsesMap) {
    usesStore->storeUses(varUsesMap);
}

std::unordered_set<variable> PKB::getVariablesUsedBy(statementNumber line) {
    return usesStore->getVariablesUsedBy(line);
}


void PKB::addConstants(std::unordered_set<constant> constants) {
    constantStore->addConstants(constants);
}

std::unordered_set<constant> PKB::getConstants() {
    return constantStore->getConstants();
}
