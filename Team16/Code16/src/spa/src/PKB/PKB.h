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

    std::unordered_set<variable> getVariablesUsedBy(statementNumber line);

    // ConstantStore methods

    void addConstants(std::unordered_set<constant> constants);

    std::unordered_set<constant> getConstants();

    // StatementStore methods

    void addStatements(std::unordered_map<statementNumber, StmtEntity> typeMap);

    std::unordered_set<statementNumber> getStatements(StmtEntity type);

    // ParentStore methods

    void storeParent(std::unordered_map<statementNumber, std::unordered_set<statementNumber>> map);

    std::unordered_set<statementNumber> getChildren(statementNumber statement);

    statementNumber getParent(statementNumber statement);

    bool isParent(statementNumber parent, statementNumber child);

    std::unordered_set<statementNumber> getChildrens(statementNumber statement);

    std::unordered_set<statementNumber> getParents(statementNumber statement);

    bool isParentStar(statementNumber parent, statementNumber child);

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
