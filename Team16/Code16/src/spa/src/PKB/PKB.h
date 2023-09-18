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

typedef std::string variable;
typedef int statementNumber;
typedef std::string possibleCombinations;
typedef std::string constant;

class PKB {
 private:
     std::unique_ptr<AssignStore> assignStore;
     std::unique_ptr<VariableStore> variableStore;
     std::unique_ptr<UsesStore> usesStore;
     std::unique_ptr<ConstantStore> constantStore;

 public:
    PKB();
    void setAssignments(std::unordered_map<statementNumber,
            std::unordered_set<possibleCombinations>> numRHSMap,
            std::unordered_map<statementNumber, variable> numLHSMap);

    std::unordered_set<statementNumber> getAllAssigns();

    std::unordered_set<statementNumber> getAssigns(variable LHS, possibleCombinations RHS);

    void addVariables(std::unordered_set<variable> variables);

    std::unordered_set<variable> getVariables();

    void addLineUsesVar(std::unordered_map<statementNumber, std::unordered_set<variable>> varUsesMap);

    void addLineUsesConst(std::unordered_map<statementNumber, std::unordered_set<constant>> constUsesMap);

    std::unordered_set<variable> getVariablesUsedBy(statementNumber line);

    std::unordered_set<constant> getConstantsUsedBy(statementNumber line);

    void addConstants(std::unordered_set<constant> constants);

    std::unordered_set<constant> getConstants();

    PKB(const PKB&) = delete;
    PKB& operator=(const PKB&) = delete;
};
