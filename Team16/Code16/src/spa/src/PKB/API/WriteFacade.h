#pragma once
#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
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
                        std::unordered_map<statementNumber, variable> numLHSMap); //TT

    // VariableStore methods TT

    void storeVariables(std::unordered_set<variable> variables);

    // UsesStore methods TT

    void storeUses(std::unordered_map<statementNumber, std::unordered_set<variable>> varUsesMap);

    // ModifiesStore methods TT

    void storeModifies(std::unordered_map<statementNumber, variable> varModifiesMap);

    // ConstantStore methods TT

    void storeConstants(std::unordered_set<constant> constants);

    // StatementStore methods

    void storeStatements(std::unordered_map<statementNumber, StmtEntity> typeMap);

    // ParentStore methods TT

    void storeParent(std::unordered_map<statementNumber, std::unordered_set<statementNumber>> map);

    // FollowStore Method TT
    void storeFollows(std::unordered_map<statementNumber, statementNumber> map);
};
