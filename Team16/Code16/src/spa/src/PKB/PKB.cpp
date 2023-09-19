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


// ParentStore methods
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

std::unordered_set<statementNumber> PKB::follows(Wildcard wildcard, StmtEntity type) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(type);
    std::unordered_set<statementNumber> result;
    for (auto const &x: relevantStmts) {
        result.insert(this->followsStore->getLeader(x));
    }
    return result;
}

statementNumber PKB::follows(statementNumber num, StmtEntity type) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(type);
    statementNumber numResult = this->followsStore->getFollower(num);
    if (relevantStmts.count(numResult)) {
        return numResult;
    }
    return 0;
}

statementNumber PKB::follows(StmtEntity type, statementNumber num) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(type);
    statementNumber numResult = this->followsStore->getLeader(num);
    if (relevantStmts.count(numResult)) {
        return numResult;
    }
    return 0;
}

std::unordered_set<statementNumber> PKB::follows(StmtEntity type, Wildcard wildcard) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(type);
    std::unordered_set<statementNumber> result;
    for (auto const &x: relevantStmts) {
        result.insert(this->followsStore->getFollower(x));
    }
    return result;
}

bool PKB::isFollow(statementNumber statement1, statementNumber statement2) {
    return this->followsStore->isFollow(statement1, statement2);
}

bool PKB::isFollow(statementNumber statement1, Wildcard wildcard) {
    return this->followsStore->isFollow(statement1, wildcard);
}

bool PKB::isFollow(Wildcard wildcard, statementNumber statement1) {
    return this->followsStore->isFollow(wildcard, statement1);
}

bool PKB::isFollow(Wildcard wildcard, Wildcard wildcard2) {
    return this->followsStore->isFollow(wildcard, wildcard2);
}

// Follows Star Methods

bool PKB::isFollowStar(statementNumber statement1, statementNumber statement2) {
    return this->followsStore->isFollowStar(statement1, statement2);
}

bool PKB::isFollowStar(statementNumber statement1, Wildcard wildcard) {
    return this->followsStore->isFollowStar(statement1, wildcard);
}

bool PKB::isFollowStar(Wildcard wildcard, statementNumber statement1) {
    return this->followsStore->isFollowStar(wildcard, statement1);
}

bool PKB::isFollowStar(Wildcard wildcard, Wildcard wildcard2) {
    return this->followsStore->isFollowStar(wildcard, wildcard2);
}


// returns all statements that are of a specified StmtEntity type and follows* any statement
std::unordered_set<statementNumber> PKB::followStar(Wildcard wildcard, StmtEntity type) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(type);
    std::unordered_set<statementNumber> result;
    for (auto const &x: relevantStmts) {
        if (this->followsStore->getLeaders(x).size() > 0) {
            result.insert(x);
        }
    }
    return result;
}

// return all statements that are of a specified StmtEntity type and follows* a specified statement
std::unordered_set<statementNumber> PKB::followStar(statementNumber num, StmtEntity type) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(type);
    std::unordered_set<statementNumber> result;
    for (auto const &x: relevantStmts) {
        if (this->followsStore->getLeaders(x).count(num)) {
            result.insert(x);
        }
    }
    return result;
}

// return all statements that are of a specified StmtEntity type and is followed* by a specified statement
std::unordered_set<statementNumber> PKB::followStar(StmtEntity type, statementNumber num) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(type);
    std::unordered_set<statementNumber> result;
    for (auto const &x: relevantStmts) {
        if (this->followsStore->getFollowers(x).count(num)) {
            result.insert(x);
        }
    }
    return result;
}

// return all statements that are of a specified StmtEntity type and is followed* by any statement
std::unordered_set<statementNumber> PKB::followStar(StmtEntity type, Wildcard wildcard) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(type);
    std::unordered_set<statementNumber> result;
    for (auto const &x: relevantStmts) {
        if (this->followsStore->getFollowers(x).size() > 0) {
            result.insert(x);
        }
    }
    return result;
}

