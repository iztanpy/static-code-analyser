#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "PKB/PKB.h"
#include <unordered_set>

class WriteFacade {
 private:
     PKB* pkb;

 public:
     explicit WriteFacade(PKB* pkbptr);

     void storeAssignments(std::vector<int> assignments);

     void storeVariables(std::vector<std::string> variables);
     void storeConstants(std::vector<std::string> constants);

     void storeUsesConst(std::unordered_map<std::string, std::unordered_set<std::string>> LHSAndRHS);
     void storeUsesVar(std::unordered_map<std::string, std::unordered_set<std::string>> LHSAndRHS);

     void storeLineUses(std::unordered_map<int, std::unordered_set<std::string>> usesVariableAndConstants);

};
