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
     PKB* pkb;

 public:
     explicit WriteFacade(PKB* pkbptr);

     void storeAssignments(std::unordered_set<int> assignments);

     void storeAssignments(std::unordered_map<statementNumber,
         std::unordered_set<possibleCombinations>> numRHSMap, std::unordered_map<statementNumber, variable> numLHSMap);

     void storeVariables(std::unordered_set<variable> variables);

     void addLineUsesVar(std::unordered_map<statementNumber, std::unordered_set<variable>> varUsesMap);

     void addLineUsesConst(std::unordered_map<statementNumber, std::unordered_set<constant>> constUsesMap);

     void storeConstants(std::unordered_set<constant> constants);

     void storeUsesConst(std::unordered_map<std::string, std::unordered_set<std::string>> LHSAndRHS);
     void storeUsesVar(std::unordered_map<std::string, std::unordered_set<std::string>> LHSAndRHS);

     void storeLineUses(std::unordered_map<int, std::unordered_set<std::string>> usesVariableAndConstants);
};
