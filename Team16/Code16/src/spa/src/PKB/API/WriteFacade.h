#pragma once
#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include "PKB/PKB.h"

typedef std::string variable;
typedef int statementNumber;
typedef std::string possibleCombinations;
typedef std::string constant;

class WriteFacade {
 private:
    PKB& pkb;

 public:
    explicit WriteFacade(PKB& pkb);

    void storeAssignments(std::unordered_map<statementNumber,
            std::unordered_set<possibleCombinations>> numRHSMap,
                        std::unordered_map<statementNumber, variable> numLHSMap);

    // VariableStore methods

    void storeVariables(std::unordered_set<variable> variables);

    // ProcedureStore methods\

    void storeProcedures(std::set<std::string> procedures);

    // UsesStore methods

    void storeUses(std::unordered_map<statementNumber, std::unordered_set<variable>> varUsesMap);

    // ModifiesStore methods

    void storeModifies(std::unordered_map<statementNumber, variable> varModifiesMap);

    // ConstantStore methods

    void storeConstants(std::unordered_set<constant> constants);

    // StatementStore methods

    void storeStatements(std::unordered_map<statementNumber, StmtEntity> typeMap);

    // ParentStore methods

    void storeParent(std::unordered_map<statementNumber, std::unordered_set<statementNumber>> map);

    // FollowStore Method
    void storeFollows(std::unordered_map<statementNumber, statementNumber> map);
};
