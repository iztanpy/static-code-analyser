#include "PKB.h"

PKB::PKB() {
    assignStore = std::make_unique<AssignStore>();
    variableStore = std::make_unique<VariableStore>();
    usesStore = std::make_unique<UsesStore>();
    constantStore = std::make_unique<ConstantStore>();
    statementStore = std::make_unique<StatementStore>();
    parentStore = std::make_unique<ParentStore>();
    followsStore = std::make_unique<FollowsStore>();
    modifiesStore = std::make_unique<ModifiesStore>();
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

std::unordered_set<statementNumber> PKB::getAssigns(variable LHS, Wildcard wildcard) {
    return assignStore->getAssigns(LHS, wildcard);
}

std::unordered_set<statementNumber> PKB::getAssigns(Wildcard wildcard, possibleCombinations RHS) {
    return assignStore->getAssigns(wildcard, RHS);
}

std::unordered_set<statementNumber> PKB::getAssigns(variable LHS, possibleCombinations RHS) {
    return assignStore->getAssigns(LHS, RHS);
}

std::unordered_set<statementNumber> PKB::getAssigns(Wildcard wildcard, Wildcard wildcard2) {
    return assignStore->getAssigns(wildcard, wildcard2);
}

std::unordered_set<std::pair<statementNumber, variable>, PairHash> PKB::getAssignPair(partialMatch partial) {
    return assignStore->getAssignPair(partial);
}

std::unordered_set<std::pair<statementNumber, variable>, PairHash> PKB::getAssignPair(Wildcard wildcard) {
    return assignStore->getAssignPair(wildcard);
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

bool PKB::isUses(statementNumber lineNumber, variable variableName) {
    return usesStore->isUses(lineNumber, variableName);
}

bool PKB::isUses(statementNumber lineNumber, Wildcard wildcard) {
    return usesStore->isUses(lineNumber);
}

std::unordered_set<variable> PKB::uses(statementNumber line) {
    return usesStore->uses(line);
}

std::unordered_set<std::pair<statementNumber, variable>, PairHash> PKB::uses(StmtEntity type) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(type);
    std::unordered_set<std::pair<statementNumber, variable>, PairHash> result;
    for (auto const& x : relevantStmts) {
        std::unordered_set<variable> variablesUsed = this->usesStore->uses(x);
        for (auto const& y : variablesUsed) {
            result.insert(std::make_pair(x, y));
        }
    }
    return result;
}

std::unordered_set<statementNumber> PKB::uses(StmtEntity type, variable variableName) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(type);
    std::unordered_set<statementNumber> result;
    for (auto const& x : relevantStmts) {
        if (this->usesStore->uses(x).count(variableName)) {
            result.insert(x);
        }
    }
    return result;
}

std::unordered_set<statementNumber> PKB::uses(StmtEntity type, Wildcard wildcard) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(type);
    std::unordered_set<statementNumber> result;
    for (auto const& x : relevantStmts) {
        if (this->usesStore->uses(x).size() > 0) {
            result.insert(x);
        }
    }
    return result;
}

// ModifiesStore methods

void PKB::storeModifies(std::unordered_map<statementNumber, variable> varModifiesMap) {
    std::unordered_map<statementNumber, std::unordered_set<variable>> ModifiesMapWithCall;

    for (auto const &x : varModifiesMap) {
        ModifiesMapWithCall[x.first].insert(x.second);
        auto parents = parentStore->getParents(x.first);
        for (auto const &y : parents) {
            ModifiesMapWithCall[y].insert(x.second);
        }
    }
    modifiesStore->storeModifies(ModifiesMapWithCall);
}

bool PKB::isModifies(statementNumber lineNumber, variable variableName) {
    return modifiesStore->isModifies(lineNumber, variableName);
}

bool PKB::isModifies(statementNumber lineNumber, Wildcard wildcard) {
    return modifiesStore->isModifies(lineNumber);
}

std::unordered_set<variable> PKB::modifies(statementNumber line) {
    return this-> modifiesStore->modifies(line);
}

std::unordered_set<statementNumber> PKB::modifies(StmtEntity type, variable variableName) {
    std::unordered_set<statementNumber> relevantStmts = statementStore->getStatements(type);
    std::unordered_set<statementNumber> result;

    for (auto const& stmt : relevantStmts) {
        const std::unordered_set<variable>& modifiedVariables = modifiesStore->modifies(stmt);
        if (modifiedVariables.find(variableName) != modifiedVariables.end()) {
            result.insert(stmt);
        }
    }

    return result;
}


std::unordered_set<statementNumber> PKB::modifies(StmtEntity type, Wildcard wildcard) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(type);
    std::unordered_set<statementNumber> result;
    for (auto const& x : relevantStmts) {
        if (this->modifiesStore->modifies(x).empty()) {
            continue;
        }
        result.insert(x);
    }
    return result;
}

std::unordered_set<std::pair<statementNumber, variable>, PairHash> PKB::modifies(StmtEntity type) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(type);
    std::unordered_set<std::pair<statementNumber, variable>, PairHash> result;
    for (auto const& x : relevantStmts) {
        std::unordered_set<variable> variablesModified = this->modifiesStore->modifies(x);
        for (auto const& y : variablesModified) {
            result.insert(std::make_pair(x, y));
        }
    }
    return result;
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

std::unordered_set<statementNumber> PKB::parent(statementNumber statement, Wildcard wildcard) {
    return parentStore->getChildren(statement);
}

statementNumber PKB::parent(Wildcard wildcard, statementNumber statement) {
    return parentStore->getParent(statement);
}

bool PKB::isParent(statementNumber parent, statementNumber child) {
    return parentStore->isParent(parent, child);
}

bool PKB::isParent(statementNumber parent, Wildcard wildcard) {
    return parentStore->isParent(parent, wildcard);
}

bool PKB::isParent(Wildcard wildcard, statementNumber child) {
    return parentStore->isParent(wildcard, child);
}

bool PKB::isParent(Wildcard wildcard, Wildcard wildcard2) {
    return parentStore->isParent(wildcard, wildcard2);
}

std::unordered_set<statementNumber> PKB::parentStar(statementNumber statement, Wildcard wildcard) {
    return parentStore->getChildrens(statement);
}

std::unordered_set<statementNumber> PKB::parentStar(Wildcard wildcard, statementNumber statement) {
    return parentStore->getParents(statement);
}

bool PKB::isParentStar(statementNumber parent, statementNumber child) {
    return parentStore->isParentStar(parent, child);
}

bool PKB::isParentStar(statementNumber parent, Wildcard wildcard) {
    return parentStore->isParentStar(parent, wildcard);
}

bool PKB::isParentStar(Wildcard wildcard, statementNumber child) {
    return parentStore->isParentStar(wildcard, child);
}

bool PKB::isParentStar(Wildcard wildcard, Wildcard wildcard2) {
    return parentStore->isParentStar(wildcard, wildcard2);
}


// FollowStore Method

void PKB::storeFollows(std::unordered_map<statementNumber, statementNumber> map) {
    followsStore->storeFollows(map);
}

std::unordered_set<statementNumber> PKB::follows(Wildcard wildcard, StmtEntity type) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(type);
    std::unordered_set<statementNumber> result;
    for (auto const &x : relevantStmts) {
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
    for (auto const &x : relevantStmts) {
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
    for (auto const &x : relevantStmts) {
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
    for (auto const &x : relevantStmts) {
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
    for (auto const &x : relevantStmts) {
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
    for (auto const &x : relevantStmts) {
        if (this->followsStore->getFollowers(x).size() > 0) {
            result.insert(x);
        }
    }
    return result;
}

