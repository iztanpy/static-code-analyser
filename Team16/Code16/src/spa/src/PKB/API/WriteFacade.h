#pragma once
#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "PKB/PKB.h"

typedef std::string variable;
typedef std::string constant;
typedef std::string possibleCombinations;
typedef int statementNumber;

class WriteFacade {
 private:
     PKB* pkb;

 public:
     explicit WriteFacade(PKB* pkbptr);

     void storeRHSAssignments(std::unordered_map<statementNumber, std::unordered_set<possibleCombinations>>assignments);
     void storeLHSAssignments(std::unordered_map<statementNumber, variable> numLHSMap);

     void storeVariables(std::unordered_set<std::string> variables);
     void storeConstants(std::unordered_set<std::string> constants);

     void addLineUsesVar(std::unordered_map<statementNumber, std::unordered_set<variable>> statementUsesMap);
     void addLineUsesConst(std::unordered_map<statementNumber, std::unordered_set<constant>> variableUsesMap);

     void storeLineUses(std::unordered_map<int, std::unordered_set<std::string>> usesVariableAndConstants);
};
