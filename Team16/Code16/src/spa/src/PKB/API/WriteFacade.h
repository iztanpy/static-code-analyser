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
         std::unordered_set<possibleCombinations>> numRHSMap, std::unordered_map<statementNumber, variable> numLHSMap);

     void storeVariables(std::unordered_set<variable> variables);

     void storeUses(std::unordered_map<statementNumber, std::unordered_set<variable>> varUsesMap);

     void storeConstants(std::unordered_set<constant> constants);
};
