#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include "Stores/VariableStore.h"
#include "Stores/AssignStore.h"
#include "Stores/UsesStore.h"
#include "Stores/ConstantStore.h"
#include "Stores/StatementStore.h"
#include "Stores/ParentStore.h"
#include "Stores/FollowsStore.h"
#include "Helper/StmtEntity.h"
#include "Helper/Wildcard.h"
#include "utils/hash_utils.h"

typedef std::string variable;
typedef int statementNumber;
typedef std::string possibleCombinations;
typedef std::string constant;
typedef std::string statementType;

class PKB {
 private:
     std::unique_ptr<AssignStore> assignStore;
     std::unique_ptr<VariableStore> variableStore;
     std::unique_ptr<UsesStore> usesStore;
     std::unique_ptr<ConstantStore> constantStore;
     std::unique_ptr<StatementStore> statementStore;
     std::unique_ptr<ParentStore> parentStore;
    std::unique_ptr<FollowsStore> followsStore;


 public:
    PKB();

    // AssignStore methods

    void setAssignments(std::unordered_map<statementNumber,
            std::unordered_set<possibleCombinations>> numRHSMap,
            std::unordered_map<statementNumber, variable> numLHSMap);

    std::unordered_set<statementNumber> getAllAssigns();

    std::unordered_set<statementNumber> getAssigns(variable LHS, possibleCombinations RHS);

    // VariableStore methods

    void addVariables(std::unordered_set<variable> variables);

    std::unordered_set<variable> getVariables();

    // UsesStore methods

    void storeUses(std::unordered_map<statementNumber, std::unordered_set<variable>> varUsesMap);

    bool isUses(statementNumber lineNumber, variable variableName);

    bool isUses(statementNumber lineNumber, Wildcard wildcard);

    std::unordered_set<statementNumber> PKB::uses(StmtEntity type, variable variableName);

    std::unordered_set<statementNumber> PKB::uses(StmtEntity type, Wildcard wildcard);

    std::unordered_set<variable> uses(statementNumber line);

    std::unordered_set<std::pair<statementNumber, variable>, PairHash> uses(StmtEntity type);

    // ConstantStore methods

    void addConstants(std::unordered_set<constant> constants);

    std::unordered_set<constant> getConstants();

    // StatementStore methods

    void addStatements(std::unordered_map<statementNumber, StmtEntity> typeMap);

    std::unordered_set<statementNumber> getStatements(StmtEntity type);

    // ParentStore methods

    void storeParent(std::unordered_map<statementNumber, std::unordered_set<statementNumber>> map);

    std::unordered_set<statementNumber> parent(statementNumber statement, Wildcard wildcard);

    statementNumber parent(Wildcard wildcard, statementNumber statement);

    bool isParent(statementNumber parent, statementNumber child);

    bool isParent(statementNumber parent, Wildcard wildcard);

    bool isParent(Wildcard wildcard, statementNumber child);

    bool isParent(Wildcard wildcard, Wildcard wildcard2);

    std::unordered_set<statementNumber> parentStar(statementNumber statement, Wildcard wildcard);

    std::unordered_set<statementNumber> parentStar(Wildcard wildcard, statementNumber statement);

    bool isParentStar(statementNumber parent, statementNumber child);

    bool isParentStar(statementNumber parent, Wildcard wildcard);

    bool isParentStar(Wildcard wildcard, statementNumber child);

    bool isParentStar(Wildcard wildcard, Wildcard wildcard2);

    // FollowStore Method
    void storeFollows(std::unordered_map<statementNumber, statementNumber> map);

    std::unordered_set<statementNumber> follows(Wildcard wildcard, StmtEntity entity);

    statementNumber follows(statementNumber num, StmtEntity entity);

    statementNumber follows(StmtEntity entity, statementNumber num);

    std::unordered_set<statementNumber> follows(StmtEntity entity, Wildcard wildcard);

    bool isFollow(statementNumber statement1, statementNumber statement2);

    bool isFollow(statementNumber statement1, Wildcard wildcard);

    bool isFollow(Wildcard wildcard, statementNumber statement1);

    bool isFollow(Wildcard wildcard, Wildcard wildcard2);

    std::unordered_set<statementNumber> followStar(Wildcard wildcard, StmtEntity entity);

    std::unordered_set<statementNumber> followStar(statementNumber num, StmtEntity entity);

    std::unordered_set<statementNumber> followStar(StmtEntity entity, statementNumber num);

    std::unordered_set<statementNumber> followStar(StmtEntity entity, Wildcard wildcard);

    bool isFollowStar(statementNumber statement1, statementNumber statement2);

    bool isFollowStar(statementNumber statement1, Wildcard wildcard);

    bool isFollowStar(Wildcard wildcard, statementNumber statement1);

    bool isFollowStar(Wildcard wildcard, Wildcard wildcard2);

    PKB(const PKB&) = delete;
    PKB& operator=(const PKB&) = delete;
};
