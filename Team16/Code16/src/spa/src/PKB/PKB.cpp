#include<stdio.h>
#include <iostream>
#include <string>
#include <unordered_set>

#include "PKB.h"


PKB::PKB() {
    assignStore = new AssignStore();
    variableStore = new VariableStore();
    usesStore = new UsesStore();
    constantStore = new ConstantStore();
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

void PKB::addLineUsesVar(std::unordered_map<statementNumber, std::unordered_set<variable>> varUsesMap) {
    usesStore->addLineUsesVar(varUsesMap);
}

void PKB::addLineUsesConst(std::unordered_map<statementNumber, std::unordered_set<constant>> constUsesMap) {
    usesStore->addLineUsesConst(constUsesMap);
}

std::unordered_set<variable> PKB::getVariablesUsedBy(statementNumber line) {
    return usesStore->getVariablesUsedBy(line);
}

std::unordered_set<constant> PKB::getConstantsUsedBy(statementNumber line) {
    return usesStore->getConstantsUsedBy(line);
}

void PKB::addConstants(std::unordered_set<constant> constants) {
    constantStore->addConstants(constants);
}

std::unordered_set<constant> PKB::getConstants() {
    return constantStore->getConstants();
}
