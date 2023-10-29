#include "PKB.h"

PKB::PKB() {
    assignStore = std::make_unique<AssignStore>();
    variableStore = std::make_unique<VariableStore>();
    usesStore = std::make_unique<RelationStore>();
    constantStore = std::make_unique<ConstantStore>();
    statementStore = std::make_unique<StatementStore>();
    parentStore = std::make_unique<ParentStore>();
    followsStore = std::make_unique<FollowsStore>();
    modifiesStore = std::make_unique<RelationStore>();
    procedureStore = std::make_unique<ProcedureStore>();
    ifStore = std::make_unique<IfStore>();
    whileStore = std::make_unique<WhileStore>();
    callStore = std::make_unique<CallStore>();
    nextStore = std::make_unique<NextStore>();
}

// AssignStore methods
std::unordered_set<statementNumber> PKB::getCommonStatements(std::unordered_set<statementNumber> set1,
    std::unordered_set<statementNumber> set2) {
    std::unordered_set<statementNumber> result;
    for (auto const& x : set1) {
        if (set2.count(x)) {
            result.insert(x);
        }
    }
    return result;
}

void PKB::setAssignments(std::unordered_map<statementNumber,
                                            std::unordered_set<partialMatch>> numRHSMap,
                         std::unordered_map<statementNumber, variable> numLHSMap) {
    assignStore->addNumLHSMap(numLHSMap);
    assignStore->addNumRHSMap(numRHSMap);
}

void PKB::setAssignments(std::unordered_map<statementNumber, std::unordered_set<partialMatch>> partialRHSMap,
                         std::unordered_map<statementNumber, full> fullRHSMap,
                         std::unordered_map<statementNumber, variable> numLHSMap) {
    assignStore->addNumLHSMap(numLHSMap);
    assignStore->addNumRHSMap(partialRHSMap);
    assignStore->storeFullPatternAssign(fullRHSMap);
}

std::unordered_set<std::pair<statementNumber, variable>, PairHash> PKB::getAssignPairPartial(partialMatch partial) {
    return assignStore->getAssignPairPartial(partial);
}

std::unordered_set<std::pair<statementNumber, variable>, PairHash> PKB::getAssignPairFull(full full) {
    return assignStore->getAssignPairFull(full);
}

std::unordered_set<statementNumber> PKB::getAssignsWcF(Wildcard lhs, full rhs) {
    return assignStore->getAssignsWcF(lhs, rhs);
}

std::unordered_set<statementNumber> PKB::getAssignsFF(full lhs, full rhs) {
    return assignStore->getAssignsFF(lhs, rhs);
}

std::unordered_set<statementNumber> PKB::getAllAssigns() {
    return assignStore->getAllAssigns();
}

std::unordered_set<statementNumber> PKB::getAssigns(variable LHS, Wildcard wildcard) {
    return assignStore->getAssigns(LHS, wildcard);
}

std::unordered_set<statementNumber> PKB::getAssigns(Wildcard wildcard, partialMatch RHS) {
    return assignStore->getAssigns(wildcard, RHS);
}

std::unordered_set<statementNumber> PKB::getAssigns(variable LHS, partialMatch RHS) {
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

// ProcedureStore methods

void PKB::addProcedures(std::set<procedure> procedures) {
    return procedureStore->addProcedures(procedures);
}

void PKB::addProcedures(std::unordered_map<procedure, std::pair<int, int>> procedures) {
    return procedureStore->addProcedures(procedures);
}

std::unordered_set<procedure> PKB::getProcedures() {
    return procedureStore->getAllProcedures();
}

// VariableStore methods

std::pair<int, int> PKB::getProcedureRange(procedure proc) {
    return procedureStore->getProcedureRange(proc);
}

void PKB::addVariables(std::unordered_set<variable> variables) {
    variableStore->addVariables(variables);
}

std::unordered_set<variable> PKB::getVariables() {
    return variableStore->getVariables();
}


// UsesStore methods

void PKB::storeUses(std::unordered_map<statementNumber, std::unordered_set<variable>> varUsesMap) {
    std::unordered_map<statementNumber, std::unordered_set<variable>> usesMapWithCall;
    for (auto const& x : varUsesMap) {
        if (usesMapWithCall.find(x.first) == usesMapWithCall.end()) {
            usesMapWithCall[x.first] = x.second;
        }
    }

    for (auto const& x : usesMapWithCall) {
        auto parents = parentStore->getParents(x.first);
        for (auto parent : parents) {
            usesMapWithCall[parent].insert(x.second.begin(), x.second.end());
        }
    }
    usesStore->storeRelation(usesMapWithCall);
}

void PKB::storeUsesProcedures(std::unordered_map<procedure, std::pair<int, int>> procedures,
                              std::unordered_map<procedure, std::unordered_set<procedure>> callTableStar) {
    usesStore->storeRelationProcedures(procedures, callTableStar);
}

void PKB::storeUsesCalls(std::unordered_map<statementNumber, procedure> calls) {
    std::unordered_map<statementNumber, std::unordered_set<statementNumber>> callsParentMap;

    for (auto const& x : calls) {
        std::unordered_set<statementNumber> parents = parentStore->getParents(x.first);

        if (parents.size() > 0) {
            callsParentMap[x.first].insert(parents.begin(), parents.end());
        }
    }
    usesStore->storeRelationCalls(calls, callsParentMap);
}

bool PKB::isUses(statementNumber lineNumber, variable variableName) {
    return usesStore->isRelation(lineNumber, variableName);
}

bool PKB::isUses(statementNumber lineNumber, Wildcard wildcard) {
    return usesStore->isRelation(lineNumber);
}

std::unordered_set<variable> PKB::uses(statementNumber line) {
    return usesStore->relates(line);
}

std::unordered_set<std::pair<statementNumber, variable>, PairHash> PKB::uses(StmtEntity type) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(type);
    std::unordered_set<std::pair<statementNumber, variable>, PairHash> result;
    for (auto const& x : relevantStmts) {
        std::unordered_set<variable> variablesUsed = this->usesStore->relates(x);
        for (auto const& y : variablesUsed) {
            result.insert(std::make_pair(x, y));
        }
    }
    return result;
}

bool PKB::isUses(procedure procedure, Wildcard wildcard) {
    return usesStore->isRelation(procedure);
}

std::unordered_set<variable> PKB::uses(procedure procedure) {
    return usesStore->relatesProcedureProc(procedure);
}

bool PKB::isUses(procedure procedure, variable variableName) {
    return usesStore->isRelation(procedure, variableName);
}

std::unordered_set<procedure> PKB::usesProcedure(Wildcard wildcard) {
    return usesStore->relatesProcedure();
}

std::unordered_set<procedure> PKB::usesProcedure(variable variableName) {
    return usesStore->relatesProcedure(variableName);
}

std::unordered_set<std::pair<procedure, variable>, PairHash> PKB::usesProcedure() {
    return usesStore->relatesProcedurePair();
}

std::unordered_set<statementNumber> PKB::uses(StmtEntity type, variable variableName) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(type);
    std::unordered_set<statementNumber> result;
    for (auto const& x : relevantStmts) {
        if (this->usesStore->relates(x).count(variableName)) {
            result.insert(x);
        }
    }
    return result;
}

std::unordered_set<statementNumber> PKB::uses(StmtEntity type, Wildcard wildcard) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(type);
    std::unordered_set<statementNumber> result;
    for (auto const& x : relevantStmts) {
        if (this->usesStore->relates(x).size() > 0) {
            result.insert(x);
        }
    }
    return result;
}

// ModifiesStore methods

void PKB::storeModifies(std::unordered_map<statementNumber, variable> varModifiesMap) {
    std::unordered_map<statementNumber, std::unordered_set<variable>> ModifiesMapWithCall;

    for (auto const& x : varModifiesMap) {
        ModifiesMapWithCall[x.first].insert(x.second);
    }

    for (auto const& x : ModifiesMapWithCall) {
        auto parents = parentStore->getParents(x.first);
        for (auto parent : parents) {
            ModifiesMapWithCall[parent].insert(x.second.begin(), x.second.end());
        }
    }
    modifiesStore->storeRelation(ModifiesMapWithCall);
}

void PKB::storeModifiesProcedures(std::unordered_map<procedure, std::pair<int, int>> procedures,
    std::unordered_map<procedure, std::unordered_set<procedure>> callTableStar) {
    modifiesStore->storeRelationProcedures(procedures, callTableStar);
}

void PKB::storeModifiesCalls(std::unordered_map<statementNumber, procedure> calls) {
    std::unordered_map<statementNumber, std::unordered_set<statementNumber>> callsParentMap;
    for (auto const& x : calls) {
        std::unordered_set<statementNumber> parents = parentStore->getParents(x.first);

        if (parents.size() > 0) {
            callsParentMap[x.first].insert(parents.begin(), parents.end());
        }
    }

    modifiesStore->storeRelationCalls(calls, callsParentMap);
}

bool PKB::isModifies(statementNumber lineNumber, variable variableName) {
    return modifiesStore->isRelation(lineNumber, variableName);
}

bool PKB::isModifies(statementNumber lineNumber, Wildcard wildcard) {
    return modifiesStore->isRelation(lineNumber);
}

std::unordered_set<variable> PKB::modifies(statementNumber line) {
    return this->modifiesStore->relates(line);
}

std::unordered_set<statementNumber> PKB::modifies(StmtEntity type, variable variableName) {
    std::unordered_set<statementNumber> relevantStmts = statementStore->getStatements(type);
    std::unordered_set<statementNumber> result;

    for (auto const& stmt : relevantStmts) {
        if (modifiesStore->isRelation(stmt, variableName)) {
            result.insert(stmt);
        }
    }
    return result;
}

std::unordered_set<statementNumber> PKB::modifies(StmtEntity type, Wildcard wildcard) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(type);
    std::unordered_set<statementNumber> result;
    for (auto const& x : relevantStmts) {
        if (this->modifiesStore->relates(x).empty()) {
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
        std::unordered_set<variable> variablesModified = this->modifiesStore->relates(x);
        for (auto const& y : variablesModified) {
            result.insert(std::make_pair(x, y));
        }
    }
    return result;
}

bool PKB::isModifies(procedure procedure, Wildcard wildcard) {
    return modifiesStore->isRelation(procedure);
}

std::unordered_set<variable> PKB::modifies(procedure procedure) {
    return modifiesStore->relatesProcedureProc(procedure);
}

bool PKB::isModifies(procedure procedure, variable variableName) {
    return modifiesStore->isRelation(procedure, variableName);
}

std::unordered_set<procedure> PKB::modifiesProcedure(Wildcard wildcard) {
    return modifiesStore->relatesProcedure();
}

std::unordered_set<procedure> PKB::modifiesProcedure(variable variableName) {
    return modifiesStore->relatesProcedure(variableName);
}

std::unordered_set<std::pair<procedure, variable>, PairHash> PKB::modifiesProcedure() {
    return modifiesStore->relatesProcedurePair();
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

// return all the entities that are nested in any statement
std::unordered_set<statementNumber> PKB::parent(Wildcard wildcard, StmtEntity entity) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(entity);
    std::unordered_set<statementNumber> result;
    for (auto const& x : relevantStmts) {
        if (!this->parentStore->getParent(x).empty()) {
            result.insert(x);
        }
    }
    return result;
}

// return the case where we have an entity that is directly nested in num
std::unordered_set<statementNumber> PKB::parent(statementNumber num, StmtEntity entity) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(entity);
    std::unordered_set<statementNumber> nestedStmts = this->parentStore->getChildren(num);
    std::unordered_set<statementNumber> result;
    for (auto const& x : nestedStmts) {
        if (relevantStmts.count(x)) {
            result.insert(x);
        }
    }
    return result;
}

// return the cases where we have a specified number directly nested in a specified entity?
std::unordered_set<statementNumber> PKB::parent(StmtEntity entity, statementNumber num) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(entity);
    std::unordered_set<statementNumber> parentStmt = this->parentStore->getParent(num);
    std::unordered_set<statementNumber> result;
    for (auto const& x : parentStmt) {
        if (relevantStmts.count(x)) {
            result.insert(x);
        }
    }
    return result;
}

// return all the specified entities that are parents of any statement
std::unordered_set<statementNumber> PKB::parent(StmtEntity entity, Wildcard wildcard) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(entity);
    std::unordered_set<statementNumber> result;
    for (auto const& x : relevantStmts) {
        if (this->parentStore->getChildren(x).size() > 0) {
            result.insert(x);
        }
    }
    return result;
}

std::unordered_set<statementNumber> PKB::parent(statementNumber statement, Wildcard wildcard) {
    return parentStore->getChildren(statement);
}

std::unordered_set<statementNumber> PKB::parent(Wildcard wildcard, statementNumber statement) {
    return parentStore->getParent(statement);
}

std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash>
PKB::parent(StmtEntity entity, StmtEntity entity2) {
    std::unordered_set<statementNumber> parentStatements = this->statementStore->getStatements(entity);
    std::unordered_set<statementNumber> childStatements = this->statementStore->getStatements(entity2);
    std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash> result;
    for (auto const& x : parentStatements) {
        for (auto const& y : childStatements) {
            if (this->parentStore->isParent(x, y)) {
                result.insert(std::make_pair(x, y));
            }
        }
    }
    return result;
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

// return all the entities that are nested
std::unordered_set<statementNumber> PKB::parentStar(Wildcard wildcard, StmtEntity entity) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(entity);
    std::unordered_set<statementNumber> result;
    for (auto const& x : relevantStmts) {
        if (this->parentStore->getParents(x).size() > 0) {
            result.insert(x);
        }
    }
    return result;
}

// return all the entities which are nested in num on any nesting level
std::unordered_set<statementNumber> PKB::parentStar(statementNumber num, StmtEntity entity) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(entity);
    std::unordered_set<statementNumber> nestedStmts = this->parentStore->getChildrens(num);
    std::unordered_set<statementNumber> result;
    for (auto const& x : nestedStmts) {
        if (relevantStmts.count(x)) {
            result.insert(x);
        }
    }
    return result;
}

// return all the entities that are parents of num on any nesting level
std::unordered_set<statementNumber> PKB::parentStar(StmtEntity entity, statementNumber num) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(entity);
    std::unordered_set<statementNumber> parentStmts = this->parentStore->getParents(num);
    std::unordered_set<statementNumber> result;
    for (auto const& x : parentStmts) {
        if (relevantStmts.count(x)) {
            result.insert(x);
        }
    }
    return result;
}

// return all the entities which are parents
std::unordered_set<statementNumber> PKB::parentStar(StmtEntity entity, Wildcard wildcard) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(entity);
    std::unordered_set<statementNumber> result;
    for (auto const& x : relevantStmts) {
        if (this->parentStore->getChildren(x).size() > 0) {
            result.insert(x);
        }
    }
    return result;
}

std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash>
PKB::parentStar(StmtEntity entity, StmtEntity entity2) {
    std::unordered_set<statementNumber> parentStatements = this->statementStore->getStatements(entity);
    std::unordered_set<statementNumber> childStatements = this->statementStore->getStatements(entity2);
    std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash> result;
    for (auto const& x : parentStatements) {
        for (auto const& y : childStatements) {
            if (this->parentStore->isParentStar(x, y)) {
                result.insert(std::make_pair(x, y));
            }
        }
    }
    return result;
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
    for (auto const& x : relevantStmts) {
        if (this->followsStore->getBefore(x) != 0) {
            result.insert(x);
        }
    }
    return result;
}

std::unordered_set<statementNumber> PKB::follows(statementNumber num, StmtEntity type) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(type);
    statementNumber numResult = this->followsStore->getAfter(num);
    if (relevantStmts.count(numResult)) {
        if (numResult != 0) {
            return {numResult};
        }
        return {};
    }
    return {};
}

std::unordered_set<statementNumber> PKB::follows(StmtEntity type, statementNumber num) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(type);
    statementNumber numResult = this->followsStore->getBefore(num);
    if (relevantStmts.count(numResult)) {
        if (numResult != 0) {
            return {numResult};
        }
        return {};
    }
    return {};
}

std::unordered_set<statementNumber> PKB::follows(StmtEntity type, Wildcard wildcard) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(type);
    std::unordered_set<statementNumber> result;
    for (auto const& x : relevantStmts) {
        if (this->followsStore->getAfter(x) != 0) {
            result.insert(x);
        }
    }
    return result;
}

std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash> PKB::follows(StmtEntity entity1,
                                                                                       StmtEntity entity2) {
    std::unordered_set<statementNumber> relevantStmts1 = this->statementStore->getStatements(entity1);
    std::unordered_set<statementNumber> relevantStmts2 = this->statementStore->getStatements(entity2);
    std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash> result;
    for (auto const& x : relevantStmts1) {
        if (x == 0) {
            continue;
        }
        if (relevantStmts2.count(this->followsStore->getAfter(x))) {
            if (this->followsStore->getAfter(x) != 0) {
                result.insert(std::make_pair(x, this->followsStore->getAfter(x)));
            }
        }
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

void PKB::storeCalls(std::unordered_map<procedure, std::unordered_set<procedure>> callTable) {
    callStore->storeCalls(callTable);
}

void PKB::storeCallsPairs(std::unordered_map<statementNumber, procedure> calls) {
    callStore->storeCallPairs(calls);
}

std::unordered_set<std::pair<statementNumber, procedure>, PairHash> PKB::getCallPairs() {
    return callStore->getCallPairs();
}

std::unordered_map<procedure, std::unordered_set<procedure>> PKB::getCallStar() {
    return callStore->getCallStar();
}

std::unordered_set<procedure> PKB::call(StmtEntity procedure, Wildcard wildcard) {
    return callStore->call(procedure, wildcard);
}

std::unordered_set<procedure> PKB::call(Wildcard wildcard, StmtEntity procedure) {
    return callStore->call(wildcard, procedure);
}

std::unordered_set<procedure> PKB::call(procedure procedure, StmtEntity proc) {
    return callStore->getCallChildren(procedure);
}

std::unordered_set<procedure> PKB::call(StmtEntity proc, procedure procedure) {
    return callStore->getCallParents(procedure);
}

bool PKB::isCall(procedure procedure, Wildcard wildcard) {
    return callStore->isCall(procedure, wildcard);
}

bool PKB::isCall(Wildcard wildcard, procedure procedure) {
    return callStore->isCall(wildcard, procedure);
}

bool PKB::isCall(procedure procedure, std::string procedure2) {
    return callStore->isCall(procedure, procedure2);
}

bool PKB::isCall(Wildcard wildcard1, Wildcard wildcard2) {
    return callStore->isCall(wildcard1, wildcard2);
}

std::unordered_set<std::pair<procedure, procedure>, PairHash> PKB::call(StmtEntity procedure1,
                                                                        StmtEntity procedure2) {
    return callStore->call(procedure1, procedure2);
}

std::unordered_set<procedure> PKB::callStar(StmtEntity procedure, Wildcard wildcard) {
    return callStore->callStar(procedure, wildcard);
}

std::unordered_set<procedure> PKB::callStar(Wildcard wildcard, StmtEntity procedure) {
    return callStore->callStar(wildcard, procedure);
}

std::unordered_set<procedure> PKB::callStar(procedure procedure, StmtEntity proc) {
    return callStore->getCallStarChildren(procedure);
}

std::unordered_set<procedure> PKB::callStar(StmtEntity proc, procedure procedure) {
    return callStore->getCallStarParents(procedure);
}

bool PKB::isCallStar(procedure procedure, Wildcard wildcard) {
    return callStore->isCallStar(procedure, wildcard);
}

bool PKB::isCallStar(Wildcard wildcard, procedure procedure) {
    return callStore->isCallStar(wildcard, procedure);
}

bool PKB::isCallStar(procedure procedure, std::string procedure2) {
    return callStore->isCallStar(procedure, procedure2);
}

bool PKB::isCallStar(Wildcard wildcard1, Wildcard wildcard2) {
    return callStore->isCallStar(wildcard1, wildcard2);
}

std::unordered_set<std::pair<procedure, procedure>, PairHash> PKB::callStar(StmtEntity procedure1,
                                                                            StmtEntity procedure2) {
    return callStore->callStar(procedure1, procedure2);
}

// returns all statements that are of a specified StmtEntity type and follows* any statement
std::unordered_set<statementNumber> PKB::followStar(Wildcard wildcard, StmtEntity type) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(type);
    std::unordered_set<statementNumber> result;
    for (auto const& x : relevantStmts) {
        if (this->followsStore->getBefores(x).size() > 0) {
            result.insert(x);
        }
    }
    return result;
}

// return all statements that are of a specified StmtEntity type and follows* a specified statement
std::unordered_set<statementNumber> PKB::followStar(statementNumber num, StmtEntity type) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(type);
    std::unordered_set<statementNumber> result;
    for (auto const& x : relevantStmts) {
        if (this->followsStore->getBefores(x).count(num)) {
            result.insert(x);
        }
    }
    return result;
}

// return all statements that are of a specified StmtEntity type and is followed* by a specified statement
std::unordered_set<statementNumber> PKB::followStar(StmtEntity type, statementNumber num) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(type);
    std::unordered_set<statementNumber> result;
    for (auto const& x : relevantStmts) {
        if (this->followsStore->getAfters(x).count(num)) {
            result.insert(x);
        }
    }
    return result;
}

// return all statements that are of a specified StmtEntity type and is followed* by any statement
std::unordered_set<statementNumber> PKB::followStar(StmtEntity type, Wildcard wildcard) {
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(type);
    std::unordered_set<statementNumber> result;
    for (auto const& x : relevantStmts) {
        if (this->followsStore->getAfters(x).size() > 0) {
            result.insert(x);
        }
    }
    return result;
}

std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash> PKB::followStar(StmtEntity entity1,
                                                                                          StmtEntity entity2) {
    std::unordered_set<statementNumber> relevantStmts1 = this->statementStore->getStatements(entity1);
    std::unordered_set<statementNumber> relevantStmts2 = this->statementStore->getStatements(entity2);
    std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash> result;
    for (auto const& x : relevantStmts1) {
        if (x == 0) {
            continue;
        }
        for (auto const& y : relevantStmts2) {
            if (y == 0) {
                continue;
            }
            if (this->followsStore->isFollowStar(x, y)) {
                result.insert(std::make_pair(x, y));
            }
        }
    }
    return result;
}

void PKB::storeIf(std::unordered_map<statementNumber, std::unordered_set<variable>> variableMap) {
    ifStore->addVariableMap(variableMap);
}

std::unordered_set<statementNumber> PKB::getIf(Wildcard wc) {
    return ifStore->getIf(wc);
}

std::unordered_set<statementNumber> PKB::getIf(variable v) {
    return ifStore->getIf(v);
}

std::unordered_set<std::pair<statementNumber, variable>, PairHash> PKB::getAllIf() {
    return ifStore->getAllIf();
}

void PKB::storeWhile(std::unordered_map<statementNumber, std::unordered_set<variable>> variableMap) {
    whileStore->addVariableMap(variableMap);
}

std::unordered_set<statementNumber> PKB::getWhile(Wildcard wc) {
    return whileStore->getWhile(wc);
}

std::unordered_set<statementNumber> PKB::getWhile(variable v) {
    return whileStore->getWhile(v);
}

std::unordered_set<std::pair<statementNumber, variable>, PairHash> PKB::getAllWhile() {
    return whileStore->getAllWhile();
}

void PKB::storeNext(std::unordered_map<statementNumber, std::unordered_set<statementNumber>> NextMap) {
    nextStore->storeNext(NextMap);
}

void PKB::storeCfg(Cfg cfg) {
    nextStore->storeCfg(cfg);
}

void PKB::storeCfgLegend(std::unordered_map<statementNumber, std::shared_ptr<CfgNode>> cfgLegend) {
    nextStore->storeCfgLegend(cfgLegend);
}

std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash> PKB::Next(StmtEntity ent1, StmtEntity ent2) {
    std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash> result;
    std::unordered_set<statementNumber> relevantStmts1 = this->statementStore->getStatements(ent1);
    std::unordered_set<statementNumber> relevantStmts2 = this->statementStore->getStatements(ent2);
    for (auto const& x : relevantStmts1) {
        for (auto const& y : relevantStmts2) {
            if (this->nextStore->isNext(x, y)) {
                result.insert(std::make_pair(x, y));
            }
        }
    }
    return result;
}

std::unordered_set<statementNumber> PKB::Next(StmtEntity ent, Wildcard) {
    std::unordered_set<statementNumber> result;
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(ent);
    for (auto const& x : relevantStmts) {
        if (this->nextStore->isNext(x, Wildcard())) {
            result.insert(x);
        }
    }
    return result;
}

std::unordered_set<statementNumber> PKB::Next(StmtEntity ent, statementNumber num) {
    std::unordered_set<statementNumber> result;
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(ent);
    for (auto const& x : relevantStmts) {
        if (this->nextStore->isNext(x, num)) {
            result.insert(x);
        }
    }
    return result;
}

std::unordered_set<statementNumber> PKB::Next(Wildcard, StmtEntity ent) {
    std::unordered_set<statementNumber> result;
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(ent);
    for (auto const& x : relevantStmts) {
        if (this->nextStore->isNext(Wildcard(), x)) {
            result.insert(x);
        }
    }
    return result;
}

std::unordered_set<statementNumber> PKB::Next(statementNumber num, StmtEntity ent) {
    std::unordered_set<statementNumber> result;
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(ent);
    for (auto const& x : relevantStmts) {
        if (this->nextStore->isNext(num, x)) {
            result.insert(x);
        }
    }
    return result;
}

bool PKB::isNext(Wildcard, Wildcard) {
    return nextStore->isNext(Wildcard(), Wildcard());
}

bool PKB::isNext(Wildcard, statementNumber num) {
    return nextStore->isNext(Wildcard(), num);
}

bool PKB::isNext(statementNumber num, Wildcard) {
    return nextStore->isNext(num, Wildcard());
}

bool PKB::isNext(statementNumber num1, statementNumber num2) {
    return nextStore->isNext(num1, num2);
}

std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash>
PKB::NextStar(StmtEntity ent1, StmtEntity ent2) {
    std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash> result;
    std::unordered_set<statementNumber> relevantStmts1 = this->statementStore->getStatements(ent1);
    std::unordered_set<statementNumber> relevantStmts2 = this->statementStore->getStatements(ent2);
    for (auto const& x : relevantStmts1) {
        for (auto const& y : relevantStmts2) {
            if (this->nextStore->isNextStar(x, y)) {
                result.insert(std::make_pair(x, y));
            }
        }
    }
    return result;
}

std::unordered_set<statementNumber> PKB::NextStar(StmtEntity ent, Wildcard) {
    std::unordered_set<statementNumber> result;
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(ent);
    for (auto const& x : relevantStmts) {
        if (this->nextStore->isNextStar(x, Wildcard())) {
            result.insert(x);
        }
    }
    return result;
}

std::unordered_set<statementNumber> PKB::NextStar(StmtEntity ent, statementNumber num) {
    std::unordered_set<statementNumber> result;
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(ent);
    for (auto const& x : relevantStmts) {
        if (this->nextStore->isNextStar(x, num)) {
            result.insert(x);
        }
    }
    return result;
}

std::unordered_set<statementNumber> PKB::NextStar(Wildcard, StmtEntity ent) {
    std::unordered_set<statementNumber> result;
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(ent);
    for (auto const& x : relevantStmts) {
        if (this->nextStore->isNextStar(Wildcard(), x)) {
            result.insert(x);
        }
    }
    return result;
}

std::unordered_set<statementNumber> PKB::NextStar(statementNumber num, StmtEntity ent) {
    std::unordered_set<statementNumber> result;
    std::unordered_set<statementNumber> relevantStmts = this->statementStore->getStatements(ent);
    for (auto const& x : relevantStmts) {
        if (this->nextStore->isNextStar(num, x)) {
            result.insert(x);
        }
    }
    return result;
}

bool PKB::isNextStar(Wildcard, Wildcard) {
    return nextStore->isNextStar(Wildcard(), Wildcard());
}

bool PKB::isNextStar(Wildcard, statementNumber num) {
    return nextStore->isNextStar(Wildcard(), num);
}

bool PKB::isNextStar(statementNumber num, Wildcard) {
    return nextStore->isNextStar(num, Wildcard());
}

bool PKB::isNextStar(statementNumber num1, statementNumber num2) {
    return nextStore->isNextStar(num1, num2);
}

void PKB::clearNextStarCache() {
    nextStore->clearCache();
}

bool PKB::modifiesStatement(statementNumber num, variable targetVariable) {
    // check if statementNumber is a assign read or procedure call
    if (statementStore->getStatements(StmtEntity::kAssign).count(num) ||
        statementStore->getStatements(StmtEntity::kRead).count(num) ||
        statementStore->getStatements(StmtEntity::kCall).count(num)) {
        return modifiesStore->isRelation(num, targetVariable);
    }
    return false;
}

bool PKB::isAffects(statementNumber statement1, statementNumber statement2) {
    if (procedureStore->getProcedureName(statement1) != procedureStore->getProcedureName(statement2)) {
        return false;
    }

    if (!statementStore->isAssign(statement1) || !statementStore->isAssign(statement2)) {
        return false;
    }

    auto modifiedVariables = modifiesStore->relates(statement1);
    auto usedVariables = usesStore->relates(statement2);

    variable modifiedVariable = *modifiedVariables.begin();
    if (usedVariables.count(modifiedVariable) == 0) {
        return false;
    }

    std::stack<statementNumber> stack;
    std::unordered_set<statementNumber> visited;

    stack.push(statement1);

    while (!stack.empty()) {
        statementNumber currentStatement = stack.top();
        stack.pop();

        if (currentStatement == statement2) {
            return true;
        }

        if (visited.count(currentStatement) == 0) {
            visited.insert(currentStatement);
            auto nextStatements = nextStore->getNext(currentStatement);
            for (auto nextStatement : nextStatements) {
                if (visited.count(nextStatement) == 0) {
                    if (this->modifiesStatement(nextStatement, modifiedVariable)) {
                        if (nextStatement == statement2) {
                            return true;
                        }
                        visited.insert(nextStatement);
                    }
                    else {
                        stack.push(nextStatement);
                    }
                }
            }
        }
    }
    return false;
}


bool PKB::isAffects(statementNumber statement1, Wildcard w) {
    if (!statementStore->isAssign(statement1)) {
        return false;
    }

    auto modifiedVariables = modifiesStore->relates(statement1);
    variable modifiedVariable = *modifiedVariables.begin();
    std::stack<statementNumber> stack;
    std::unordered_set<statementNumber> visited;

    stack.push(statement1);

    while (!stack.empty()) {
        statementNumber currentStatement = stack.top();
        stack.pop();

        if (visited.count(currentStatement) == 0) {
            visited.insert(currentStatement);
            auto nextStatements = nextStore->getNext(currentStatement);
            for (auto nextStatement : nextStatements) {
                if (visited.count(nextStatement) == 0) {
                    if (this->modifiesStatement(nextStatement, modifiedVariable)&& statementStore->isAssign(nextStatement)) {
                        return true;
                    }
                    else {
                        stack.push(nextStatement);
                    }
                }
            }
        }
    }
    return false;
}

bool PKB::isAffects(Wildcard w, statementNumber statement2) {
    if (!statementStore->isAssign(statement2)) {
        return false;
    }
    auto usedVariables = usesStore->relates(statement2);

    std::stack<statementNumber> stack;
    std::unordered_set<statementNumber> visited;

    stack.push(statement2);

    while (!stack.empty()) {
        statementNumber currentStatement = stack.top();
        stack.pop();

        if (visited.count(currentStatement) == 0) {
            visited.insert(currentStatement);
            auto previousStatements = nextStore->getNextReverse(currentStatement);
            for (auto previousStatement : previousStatements) {
                if (visited.count(previousStatement) == 0) {
                    auto modifiedVariables = modifiesStore->relates(previousStatement);

                    if (statementStore->isAssign(statement2)) {
                        variable modifiedVariable = *modifiedVariables.begin();
                        if (usedVariables.count(modifiedVariable) != 0) {
                            return true;
                        }
                    }
                    stack.push(previousStatement);
                }
            }
        }
    }
    return false;
}

bool PKB::isAffects(Wildcard w, Wildcard w2) {
    std::unordered_set<statementNumber> assignStatements = statementStore->getStatements(StmtEntity::kAssign);
    for (auto assignStatement : assignStatements) {
        if (isAffects(assignStatement, Wildcard())) {
            return true;
        }
    }
    return false;
}

std::unordered_set<std::pair<statementNumber, variable>, PairHash>
        PKB::getStatementsAndVariable(StmtEntity type) {
    // if type is print or call or read,
    std::unordered_set<std::pair<statementNumber, variable>, PairHash> statementsAndVariable;
    auto typeStatements = statementStore->getStatements(type);
    switch (type) {
        case StmtEntity::kPrint:
            for (auto statement : typeStatements) {
                auto variables = usesStore->relates(statement);
                for (auto variable : variables) {
                    statementsAndVariable.insert(std::make_pair(statement, variable));
                }
            }
            return statementsAndVariable;
        case StmtEntity::kRead:
            for (auto statement : typeStatements) {
                auto variables = modifiesStore->relates(statement);
                for (auto variable : variables) {
                    statementsAndVariable.insert(std::make_pair(statement, variable));
                }
            }
            return statementsAndVariable;
        case StmtEntity::kCall:
            return callStore->getCallPairs();
        default:
            return std::unordered_set<std::pair<statementNumber, variable>, PairHash>();
    }
}
