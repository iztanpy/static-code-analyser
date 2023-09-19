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
    statementStore = std::make_unique<StatementStore>();
    parentStore = std::make_unique<ParentStore>();
    followsStore = std::make_unique<FollowsStore>();
}

// AssignStore methods

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


// VariableStore methods

void PKB::addVariables(std::unordered_set<variable> variables) {
    variableStore->addVariables(variables);
}

std::unordered_set<variable> PKB::getVariables() {
    return variableStore->getVariables();
}


// UsesStore methods

void PKB::storeUses(std::unordered_map<statementNumber, std::unordered_set<variable>> varUsesMap) {
    usesStore->storeUses(varUsesMap);
}

std::unordered_set<variable> PKB::getVariablesUsedBy(statementNumber line) {
    return usesStore->getVariablesUsedBy(line);
}

// ConstantStore methods

void PKB::addConstants(std::unordered_set<constant> constants) {
    constantStore->addConstants(constants);
}

std::unordered_set<constant> PKB::getConstants() {
    return constantStore->getConstants();
}

// StatementStore methods

void PKB::addStatements(std::unordered_map<statementNumber, StmtEntity> typeMap) {
    statementStore->addStatements(typeMap);
}

std::unordered_set<statementNumber> PKB::getStatements(StmtEntity type) {
    return statementStore->getStatements(type);
}

void PKB::storeParent(std::unordered_map<statementNumber, std::unordered_set<statementNumber>> map) {
    parentStore->storeParent(map);
}

std::unordered_set<statementNumber> PKB::getChildren(statementNumber statement) {
    return parentStore->getChildren(statement);
}

statementNumber PKB::getParent(statementNumber statement) {
    return parentStore->getParent(statement);
}

bool PKB::isParent(statementNumber parent, statementNumber child) {
    return parentStore->isParent(parent, child);
}

std::unordered_set<statementNumber> PKB::getChildrens(statementNumber statement) {
    return parentStore->getChildrens(statement);
}

std::unordered_set<statementNumber> PKB::getParents(statementNumber statement) {
    return parentStore->getParents(statement);
}

bool PKB::isParentStar(statementNumber parent, statementNumber child) {
    return parentStore->isParentStar(parent, child);
}


// FollowStore Method

void PKB::storeFollows(std::unordered_map<statementNumber, statementNumber> map) {
    followsStore->storeFollows(map);
}

std::unordered_set<statementNumber> PKB::Follow(variable wildcard, StmtEntity type) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(type);
    std::unordered_set<statementNumber> result;
    for (auto const &x: relevantStmts) {
        result.insert(this->followsStore->getLeader(x));
    }
    return result;
}
